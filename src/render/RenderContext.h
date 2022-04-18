#pragma once

#include <nativeui/NativeUI.h>
#include <command/RenderCommand_Draw.h>

namespace SimpleGameEngine {

	struct RenderContextCreateDesc {
		NativeUIWindow* window = nullptr;
	};

	class RenderContext : public NonCopyable {
	public:
		using CreateDesc = RenderContextCreateDesc;
		static RenderContext* create(RenderContextCreateDesc& desc);

		void render(RenderCommand_Draw& cmd);

		RenderContext(RenderContextCreateDesc& desc);
		virtual ~RenderContext() = default;

	protected:
		virtual void onBeginRender() {};
		virtual void onRender(RenderCommand_Draw& cmd) {};
		virtual void onEndRender() {};
		virtual void onClearBuffers() {}
		virtual void onSwapBuffers() {}
	};
}