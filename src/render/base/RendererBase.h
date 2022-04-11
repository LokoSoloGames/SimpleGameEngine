#pragma once

#include <sgebase.h>
#include <nativeui/NativeUI.h>

namespace SimpleGameEngine {

	class RendererBase : public NonCopyable {
	public:
		struct CreateDesc {
			int platform;
			Vec2f wh;
		};

		void init(NativeUIWindow &window, CreateDesc &desc) { onInit(window, desc); }
		void render() { onRender(); }
		void cleanUp() { onCleanUp(); }

	protected:
		virtual void onInit(NativeUIWindow &window, CreateDesc &desc) {}
		virtual void onRender() {}
		virtual void onCleanUp() {}
	};
}