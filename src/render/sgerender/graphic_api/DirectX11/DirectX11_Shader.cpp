#include "DirectX11_Shader.h"
#include "DirectX11_Renderer.h"
#include "DirectX11_RenderContext.h"

namespace SimpleGameEngine {
	DirectX11_Shader::DirectX11_Shader(StrView filename) : Base(filename) {
		auto* proj = ProjectSettings::instance();
		TempString passPath;

		size_t n = _info.passes.size();
		_passes.reserve(n);
		for (size_t i = 0; i < n; i++) {
			FmtTo(passPath, "{}/{}/dx11/pass{}", proj->importedPath(), filename, i);
			auto* pass = new MyPass(this, passPath, _info.passes[i]);
			_passes.emplace_back(pass);
		}
	}

	void DirectX11_Shader::_loadStageFile(StrView passPath, ShaderStageMask stageMask, Vector<u8>& outBytecode, ShaderStageInfo& outInfo) {
		auto* profile = Util::getDxStageProfile(stageMask);

		auto filename = Fmt("{}/{}.bin", passPath, profile);
		File::readFile(filename, outBytecode);

		filename += ".json";
		JsonUtil::readFile(filename, outInfo);
	}

	void DirectX11_Shader::MyVertexStage::load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev) {
		_loadStageFile(passPath, stageMask(), _bytecode, _info);
		auto hr = dev->CreateVertexShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_Shader::MyPixelStage::load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev) {
		_loadStageFile(passPath, stageMask(), _bytecode, _info);
		auto hr = dev->CreatePixelShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_Shader::MyVertexStage::bind(DirectX11_RenderContext* ctx) {
		auto* dc = ctx->renderer()->d3dDeviceContext();
		if (!_shader) throw SGE_ERROR("dx shader is null");
		dc->VSSetShader(_shader, 0, 0);
	}

	void DirectX11_Shader::MyPixelStage::bind(DirectX11_RenderContext* ctx) {
		auto* dc = ctx->renderer()->d3dDeviceContext();
		if (!_shader) throw SGE_ERROR("dx shader is null");
		dc->PSSetShader(_shader, 0, 0);
	}

	DirectX11_Shader::MyPass::MyPass(DirectX11_Shader* shader, StrView passPath, ShaderInfo::Pass& info)
		: Base(shader, info)
	{
		_vertexStage = &_myVertexStage;
		_pixelStage = &_myPixelStage;

		auto* renderer = DirectX11_Renderer::current();
		auto* dev = renderer->d3dDevice();

		if (info.vsFunc.size()) { _myVertexStage.load(this, passPath, dev); }
		if (info.psFunc.size()) { _myPixelStage.load(this, passPath, dev); }
	}
}