#pragma once

#include <Renderer.h>
#include "OpenGL_Common.h"

#if SGE_RENDER_HAS_OPENGL
namespace SimpleGameEngine {
	class OpenGL_Win32_Renderer : public Renderer { // TODO Refactor to OpenGL_Base_Renderer
	public:
		static OpenGL_Win32_Renderer* current() { return static_cast<OpenGL_Win32_Renderer*>(_current); }

	protected:
		virtual RenderContext*	onCreateContext(RenderContextCreateDesc& desc);
	};
}
#endif