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
		virtual void onBeginRender() = 0;
		virtual void onRender(RenderCommand_Draw& cmd) = 0;
		virtual void onEndRender() = 0;
		virtual void onClearBuffers() = 0;
		virtual void onSwapBuffers()  = 0;
	};
}