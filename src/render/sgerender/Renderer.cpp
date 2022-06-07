#include "Renderer.h"

#include "graphic_api/DirectX11/DirectX11_Renderer.h"

namespace SimpleGameEngine {

	Renderer* Renderer::_current = nullptr;

	Renderer::CreateDesc::CreateDesc() {
#if SGE_OS_WINDOWS
		apiType = ApiType::DX11;
#else
		apiType = ApiType::None;
#endif

		multithread = false;
	}

	Renderer* Renderer::create(CreateDesc& desc) {
		Renderer* p = nullptr;
		switch (desc.apiType) {
			case ApiType::DX11: p = new DirectX11_Renderer(desc); break;
			default: throw SGE_ERROR("unsupport graphic api");
		}
		return p;
	}

	Renderer::Renderer() {
		SGE_ASSERT(_current == nullptr);
		_current = this;
		_vsync = true;
	}

	Renderer::~Renderer() {
		SGE_ASSERT(_shaders.size() == 0);
		SGE_ASSERT(_current == this);
		_current = nullptr;
	}

	SPtr<Shader> Renderer::createShader(StrView filename) {
		TempString tmpName = filename;

		auto it = _shaders.find(tmpName.c_str());
		if (it != _shaders.end()) {
			return it->second;
		}

		auto s = onCreateShader(tmpName);
		_shaders[tmpName.c_str()] = s.ptr();
		return s;
	}

	void Renderer::onShaderDestroy(Shader* shader) {
		_shaders.erase(shader->filename().c_str());
	}

}
