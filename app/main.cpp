#include <nativeui/NativeUI.h>
#include <sgecore/log/Log.h>
#include <sgecore/file/FilePath.h>
#include <Renderer.h>
#include <RenderContext.h>
#include <command/RenderMesh.h>
#include <command/RenderCommand_Draw.h>

namespace SimpleGameEngine {

	class MainWin : public NativeUIWindow {
		using Base = NativeUIWindow;
	public:
		void onCreate(CreateDesc& desc) {
			Base::onCreate(desc);

			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;

			_renderContext.reset(RenderContext::create(renderContextDesc));

			Mesh mesh;
			Position3f pos1 = {0.0f, 0.5f, 0.0f};
			Color4b color1 = {255, 0, 0, 255};
			mesh.add(pos1, color1);
			Position3f pos2 = {0.5f, -0.5f, 0.0f};
			Color4b color2 = {0, 255, 0, 255};
			mesh.add(pos2, color2);
			Position3f pos3 = {-0.5f, -0.5f, 0.0f};
			Color4b color3 = {0, 0, 255, 255};
			mesh.add(pos3, color3);
			_renderMesh.reset(new RenderMesh(mesh));
			_shaderPass.reset(new ShaderPass(_renderMesh.get(), L"shaders.shader"));
		}

		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}

		virtual void onDraw() {
			Base::onDraw();
			if (_renderContext) {
				RenderCommand_Draw cmd;
				cmd.primitive = PrimitiveTopology::TriangleList;
				cmd.renderMesh = _renderMesh.get();
				cmd.shaderPass = _shaderPass.get();
				_renderContext->render(cmd);
			}
			drawNeeded();
		}
		UPtr<RenderMesh> _renderMesh;
		UPtr<ShaderPass> _shaderPass;
		UPtr<RenderContext>	_renderContext;
	};

	class EditorApp : public NativeUIApp {
		using Base = NativeUIApp;
	public:
		virtual void onCreate(CreateDesc& desc) override {
			Base::onCreate(desc);

			{
				String file = getExecutableFilename();
				String path = FilePath::getDir(file);
				path.append("/../assets");
				setCurrentDir(path);
			}

			Renderer::CreateDesc renderDesc;
			//renderDesc.apiType = OpenGL;
			Renderer::create(renderDesc);

			NativeUIWindow::CreateDesc winDesc;
			winDesc.isMainWindow = true;
			winDesc.rect = {10, 10, 1024, 768};
			winDesc.ownContext = renderDesc.apiType == Renderer::ApiType::OpenGL;
			_mainWin.create(winDesc);
			_mainWin.setWindowTitle("Simple Game Engine Editor");
		}

		virtual void onUpdate() override {
			_mainWin.onUpdate();
		}

	private:
		MainWin			_mainWin;
	};
}

int main() {
	SimpleGameEngine::EditorApp app;
	SimpleGameEngine::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}