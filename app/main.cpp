#include "nativeui/NativeUI.h"
#include <log/Log.h>

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
			_mainWin.create(winDesc);
			_mainWin.setWindowTitle("Simple Game Engine Editor");

			SGE_LOG("Hello {}", 10);

//		_renderer.create(_mainWin);
		}

	private:
		MainWin		_mainWin;
//	Renderer	_renderer;
	};

}

int main() {
	SimpleGameEngine::EditorApp app;
	SimpleGameEngine::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}
