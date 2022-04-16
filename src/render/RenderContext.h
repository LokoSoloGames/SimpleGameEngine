#pragma once
#include <nativeui/NativeUI.h>

namespace SimpleGameEngine {

	struct RenderContextCreateDesc {
		NativeUIWindow*	window = nullptr;
	};

	class RenderContext : public NonCopyable {
	public:
		using CreateDesc = RenderContextCreateDesc;
		static RenderContext*	create(RenderContextCreateDesc& desc);

		void render();

		RenderContext(RenderContextCreateDesc& desc);
		virtual ~RenderContext() = default;

	protected:
		virtual void onBeginRender() {};
		virtual void onRender() {};
		virtual void onEndRender() {};
		virtual void onClearBuffers() {}
		virtual void onSwapBuffers() {}
	};


}