#include "ShaderCompiler_DX11.h"
#include "ShaderParser.h"

namespace SimpleGameEngine {
	struct DX11ShaderUtil {
		static RenderDataType getDataType(D3D11_SIGNATURE_PARAMETER_DESC& desc) {
			auto c = getComponentCount(desc.Mask);
			switch (desc.ComponentType) {
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
				return RenderDataType::Float32 + c - 1;
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UINT32:
				return RenderDataType::UInt32 + c - 1;
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_SINT32:
				return RenderDataType::Int32 + c - 1;
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UNKNOWN:
				throw SGE_ERROR("Unknown D3D Component Type");
			default:
				throw SGE_ERROR("Invalid D3D10 Component Type: {}", desc.ComponentType);
			}
		}

		static u8 getComponentCount(BYTE& mask) {
			return
				((mask & (1 << 0)) >> 0) +
				((mask & (1 << 1)) >> 1) +
				((mask & (1 << 2)) >> 2) +
				((mask & (1 << 3)) >> 3);
		}

		// Maybe cannot use RenderDataType because ShaderDataType includes more variety
		static RenderDataType getDataType(D3D11_SHADER_TYPE_DESC& desc) {
			auto c = getComponentCount(desc);
			switch (desc.Type) {
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
				return RenderDataType::Int32 + c - 1;
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
				return RenderDataType::Float32 + c - 1;
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_UINT:
				return RenderDataType::UInt32 + c - 1;
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_UINT8:
				return RenderDataType::Int8 + c - 1;
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_DOUBLE:
				return RenderDataType::Float64 + c - 1;
			default:
				throw SGE_ERROR("Unhandled D3D SHADER VARIABLE TYPE: {}", desc.Type);
			}
		}

		static u32 getComponentCount(D3D11_SHADER_TYPE_DESC& desc) {
			switch (desc.Class) {
			case _D3D_SHADER_VARIABLE_CLASS::D3D_SVC_SCALAR:
				return 1;
			case _D3D_SHADER_VARIABLE_CLASS::D3D10_SVC_VECTOR:
				return desc.Columns;
			default:
				throw SGE_ERROR("Unhandled D3D SHADER VARIABLE CLASS: {}", desc.Class);
			}
		}
	};

	void ShaderCompiler_DX11::compile(const StrView& outFileName, const ShaderStageMask& shaderStage, const StrView& srcFileName, const StrView& entryFunc, ShaderLayout& layout) {
		TempStringA entryPoint = entryFunc;

		MemMapFile memmap;
		memmap.open(srcFileName);

		auto hlsl = memmap.span();

		UINT flags1 = 0; // D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
		UINT flags2 = 0;

#if _DEBUG
		//flags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ComPtr<ID3DBlob>	byteCode;
		ComPtr<ID3DBlob>	errorMsg;

		auto profile = Util::getDxStageProfile(shaderStage);

		auto hr = D3DCompile2(
			hlsl.data(), hlsl.size(), memmap.filename().c_str(),
			nullptr, nullptr,
			entryPoint.c_str(),
			profile,
			flags1, flags2, 0, nullptr, 0,
			byteCode.ptrForInit(),
			errorMsg.ptrForInit());

		if (FAILED(hr)) {
			throw SGE_ERROR("HRESULT={}\n Error Message: {}", hr, Util::toStrView(errorMsg));
		}
		
		ComPtr<DX11_ID3DShaderReflection> reflection;
		hr = D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), IID_PPV_ARGS(reflection.ptrForInit()));
		Util::throwIfError(hr);
		layout.clear();
		layout.profile = profile;
		createLayout(reflection, layout);

		Directory::create(outFileName);

		auto shaderFileName = Fmt("{}/{}.bin", outFileName, profile);
		File::writeFile(shaderFileName, Util::toSpan(byteCode), false);

		auto layoutFileName = Fmt("{}/{}.bin.json", outFileName, profile);
		File::writeFile(layoutFileName, layout.toJson(), false);
	}

	void ShaderCompiler_DX11::createLayout(ComPtr<DX11_ID3DShaderReflection>& reflection, ShaderLayout& layout) {

		HRESULT hr;
		D3D11_SHADER_DESC shaderDesc;
		hr = reflection->GetDesc(&shaderDesc);
		Util::throwIfError(hr);

		{
			D3D11_SIGNATURE_PARAMETER_DESC desc;
			for (u32 i = 0; i < shaderDesc.InputParameters; i++) {
				hr = reflection->GetInputParameterDesc(i, &desc);
				Util::throwIfError(hr);
				auto& inputLayout = layout.inputs.emplace_back();
				inputLayout.attrId = Fmt("{}{}", desc.SemanticName, desc.SemanticIndex);
				inputLayout.dataType = DX11ShaderUtil::getDataType(desc);
			}
		}

		{
			D3D11_SHADER_BUFFER_DESC bufferDesc;
			D3D11_SHADER_VARIABLE_DESC varDesc;
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			D3D11_SHADER_TYPE_DESC typeDesc;
			for (u32 i = 0; i < shaderDesc.ConstantBuffers; i++) {
				auto* constBuffer = reflection->GetConstantBufferByIndex(0);
				hr = constBuffer->GetDesc(&bufferDesc);
				Util::throwIfError(hr);
				if (bufferDesc.Type == D3D_CBUFFER_TYPE::D3D11_CT_CBUFFER) {
					auto& bufferLayout = layout.uniformBuffers.emplace_back();
					bufferLayout.name = bufferDesc.Name;
					bufferLayout.dataSize = bufferDesc.Size;
					hr = reflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);
					Util::throwIfError(hr);
					bufferLayout.bindPoint = bindDesc.BindPoint;
					bufferLayout.bindCount = bindDesc.BindCount;

					for (u32 j = 0; j < bufferDesc.Variables; j++) {
						auto* variable = constBuffer->GetVariableByIndex(j);
						hr = variable->GetDesc(&varDesc);
						Util::throwIfError(hr);
						auto& varLayout = bufferLayout.variables.emplace_back();
						varLayout.name = varDesc.Name;
						varLayout.offset = varDesc.StartOffset;
						hr = variable->GetType()->GetDesc(&typeDesc);
						varLayout.dataType = DX11ShaderUtil::getDataType(typeDesc);
					}
				}
			}
		}
	}
}