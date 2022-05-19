#include "ShaderCompiler.h"
#include "ShaderParser.h"

namespace SimpleGameEngine {
	void ShaderCompiler::compile(const StrView& fileName) {
		MemMapFile mm;
		mm.open(fileName);
		ShaderParser parser;
		ShaderParser::ShaderManifest manifest = parser.parseShader(mm);

		_dirPath = Fmt("shaders/{}", fileName);
		compiledShader.clear();

		for (size_t i = 0; i < manifest.pass.size(); i++) {
			if (manifest.pass[i].vsName.size()) {
				compileVS(manifest.shader, manifest.shaderName, manifest.pass[i].vsName);
			}
			if (manifest.pass[i].psName.size()) {
				compilePS(manifest.shader, manifest.shaderName, manifest.pass[i].psName);
			}
		}
	}

	void ShaderCompiler::compileVS(const StrView& shader, const StrView& alias, const StrView& vsName) {
		if (vsName.size()) {
			auto passName = Fmt("dx11_{}_vs", vsName).c_str();
			auto x = compiledShader.find(passName);
			if (x == compiledShader.end()) {
				HRESULT hr;
				ComPtr<DX11_ID3DBlob> bytecode;
				ComPtr<DX11_ID3DBlob> errorMsg;

				hr = D3DCompile(shader.data(), shader.size(), alias.data(), 0, 0, vsName.data(), "vs_4_0", 0, 0, bytecode.ptrForInit(), errorMsg.ptrForInit());
				DX11Util::throwIfError(hr);

				ComPtr<DX11_ID3DShaderReflection> reflection;
				hr = D3DReflect(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(reflection.ptrForInit()));
				DX11Util::throwIfError(hr);

				auto info = createInfo(reflection);
				compiledShader.insert_or_assign(passName, info);

				FileStream fs;
				fs.openWrite(Fmt("{}/{}.bin", _dirPath, passName), true);
				u8* start = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
				fs.writeBytes(Span<const u8>(start, bytecode->GetBufferSize()));
			}
		}
	}

	Shaderinfo ShaderCompiler::createInfo(ComPtr<DX11_ID3DShaderReflection>& reflection) {
		Shaderinfo info;

		HRESULT hr;
		D3D11_SHADER_DESC shaderDesc;
		hr = reflection->GetDesc(&shaderDesc);
		DX11Util::throwIfError(hr);
		
		if (shaderDesc.InputParameters > 0) {
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			for (u8 i = 0; i < shaderDesc.InputParameters; i++) {
				hr = reflection->GetInputParameterDesc(i, &paramDesc);
				DX11Util::throwIfError(hr);
				Shaderinfo::InputInfo inputInfo;
				inputInfo.attrId = Fmt("{}{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
				inputInfo.dataType = DX11ShaderUtil::getDataType(paramDesc);
			}
		}
		
		// TODO: Hmm..........
	}

	struct DX11ShaderUtil {
		static RenderDataType getDataType(D3D11_SIGNATURE_PARAMETER_DESC& desc) {
			auto c = getComponentCount(desc.Mask);
			switch (desc.ComponentType) {
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
				return static_cast<RenderDataType>(enumInt(RenderDataType::Float32) + c - 1);
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UINT32:
				return static_cast<RenderDataType>(enumInt(RenderDataType::UInt32) + c - 1);
			case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_SINT32:
				return static_cast<RenderDataType>(enumInt(RenderDataType::Int32) + c - 1);
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
}