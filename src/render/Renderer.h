#pragma once

#include <sgebase.h>

#if SGE_OS_WINDOWS

#include "DirectX11/DirectX11Renderer.h"
namespace SimpleGameEngine {
	using Renderer_Impl	= DirectX11Renderer;
}

#else
#error "unsupported platform"

#endif

namespace SimpleGameEngine {
	class Renderer {
	public:
		static RendererBase* create(RendererBase::CreateDesc& desc){
			return new Renderer_Impl();
		}
	};
}