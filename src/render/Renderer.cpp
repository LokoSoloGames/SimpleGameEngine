#include "Renderer.h"

#include "DirectX11/DirectX11Renderer.h"

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
			case ApiType::DX11: p = new DirectX11Renderer(desc); break;
			default: throw SGE_ERROR("unsupport graphic api");
		}
		SGE_LOG("Created Renderer");
		return p;
	}

	Renderer::Renderer() {
		SGE_ASSERT(_current == nullptr);
		_current = this;
		_vsync = true;
	}

	Renderer::~Renderer() {
		SGE_ASSERT(_current == this);
		_current = nullptr;
	}

}
