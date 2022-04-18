#include "RenderContext.h"
#include "Renderer.h"

namespace SimpleGameEngine {

	RenderContext* RenderContext::create(RenderContextCreateDesc& desc) {
		return Renderer::current()->onCreateContext(desc);
	}

	void RenderContext::render(RenderCommand_Draw& cmd) {
		onBeginRender();
		onClearBuffers();
		onRender(cmd);
		onSwapBuffers();
		onEndRender();
	}

	RenderContext::RenderContext(RenderContextCreateDesc& desc) {

	}
}