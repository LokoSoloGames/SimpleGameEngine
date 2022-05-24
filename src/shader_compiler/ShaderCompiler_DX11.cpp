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
				throw SGE_ERROR("Invalid D3D10 Component Type: {}", desc.ComponentType);;
			}
		}

		static u8 getComponentCount(BYTE& mask) {
			return
				((mask & (1 << 0)) >> 0) +
				((mask & (1 << 1)) >> 1) +
				((mask & (1 << 2)) >> 2) +
				((mask & (1 << 3)) >> 3);
		}
	};

	void ShaderCompiler_DX11::compile(const StrView& outFileName, const ShaderStage& shaderStage, const StrView& srcFileName, const StrView& entryFunc, ShaderLayout& layout) {
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
		
		if (shaderDesc.InputParameters > 0) {
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			for (u8 i = 0; i < shaderDesc.InputParameters; i++) {
				hr = reflection->GetInputParameterDesc(i, &paramDesc);
				Util::throwIfError(hr);
				ShaderLayout::InputLayout inputLayout;
				inputLayout.attrId = Fmt("{}{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
				inputLayout.dataType = DX11ShaderUtil::getDataType(paramDesc);
			}
		}
		
		// TODO: Hmm..........
	}
}