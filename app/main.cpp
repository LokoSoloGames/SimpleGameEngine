#include <nativeui/NativeUI.h>
#include <sgecore/log/Log.h>
#include "Renderer.h"

namespace SimpleGameEngine {

	class MainWin : public NativeUIWindow {
	public:
		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}
	};

	class EditorApp : public NativeUIApp {
		using Base = NativeUIApp;
	public:
		virtual void onCreate(CreateDesc& desc) override {
			Base::onCreate(desc);

			NativeUIWindow::CreateDesc winDesc;
			winDesc.isMainWindow = true;
			winDesc.rect = {10, 10, 1024, 768};
			_mainWin.create(winDesc);
			_mainWin.setWindowTitle("Simple Game Engine Editor");

			RendererBase::CreateDesc renderDesc;
			renderDesc.platform = 1; // Define Platform
			renderDesc.wh = {1024, 768};
			_renderer = Renderer::create(renderDesc);
			_renderer->init(_mainWin, renderDesc);
		}

		virtual void onUpdate() override {
			_renderer->render();
		}

		virtual void onQuit() override{
			_renderer->cleanUp();
			Base::onQuit();
		}

	private:
		MainWin			_mainWin;
		RendererBase*	_renderer;
	};
}

int main() {
	SimpleGameEngine::EditorApp app;
	SimpleGameEngine::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}