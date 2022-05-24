#include <nativeui/NativeUI.h>
#include <sgecore.h>
#include <sgerender.h>
#include <sgerender/mesh/RenderMesh.h>
#include <sgerender/command/RenderCommand_Draw.h>
#include <sgerender/mesh/reader/WavefrontObjLoader.h>

namespace SimpleGameEngine {

	class MainWin : public NativeUIWindow {
		using Base = NativeUIWindow;
	public:
		void onCreate(CreateDesc& desc) {
			Base::onCreate(desc);

			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;
			
			EditMesh editMesh;
			/*Tuple3f pos1 = {0.0f, 0.5f, 0.0f};
			Color4b color1 = {255, 0, 0, 255};
			mesh.pos.emplace_back(pos1);
			mesh.color.emplace_back(color1);
			Tuple3f pos2 = {0.5f, -0.5f, 0.0f};
			Color4b color2 = {0, 255, 0, 255};
			mesh.pos.emplace_back(pos2);
			mesh.color.emplace_back(color2);
			Tuple3f pos3 = {-0.5f, -0.5f, 0.0f};
			Color4b color3 = {0, 0, 255, 255};
			mesh.pos.emplace_back(pos3);
			mesh.color.emplace_back(color3);*/
			WavefrontObjLoader::loadFile(editMesh, "bunny.obj");
			for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
				editMesh.color.emplace_back(255, 255, 255, 255);
			}

			// the current shader has no uv or normal
			editMesh.uv[0].clear();
			editMesh.normal.clear();

			_renderMesh.create(editMesh);
			_shaderPass.reset(new ShaderPass(_renderMesh, L"shaders.shader"));

			_renderContext.reset(RenderContext::create(renderContextDesc));
		}

		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}

		virtual void onDraw() {
			Base::onDraw();
			if (_renderContext) {
				RenderCommand_Draw cmd;
				cmd.renderMesh = &_renderMesh.subMeshes()[0];
				cmd.shaderPass = _shaderPass.get();
				_renderContext->render(cmd);
			}
			drawNeeded();
		}
		RenderMesh _renderMesh;
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
				String path = FilePath::dirname(file);
				path.append("/../assets");
				Directory::setCurrent(path);
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