#include "RenderContext.h"
#include "Renderer.h"

namespace SimpleGameEngine {

	RenderContext* RenderContext::create(RenderContextCreateDesc& desc) {
		SGE_LOG("Singleton create render context");
		return Renderer::current()->onCreateContext(desc);
	}

	void RenderContext::render() {
		onBeginRender();
		onClearBuffers();
		onRender();
		onSwapBuffers();
		onEndRender();
	}

	RenderContext::RenderContext(RenderContextCreateDesc& desc) {

	}
}