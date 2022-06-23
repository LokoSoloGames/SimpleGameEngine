#include <nativeui/NativeUI.h>
#include <sgecore.h>
#include <sgerender.h>
#include <sgerender/mesh/RenderMesh.h>
#include <sgerender/command/RenderCommand.h>
#include <sgerender/mesh/reader/WavefrontObjLoader.h>

namespace SimpleGameEngine {

	class MainWin : public NativeUIWindow {
		using Base = NativeUIWindow;
	public:
		void onCreate(CreateDesc& desc) {
			Base::onCreate(desc);

			auto* renderer = Renderer::current(); 
			{
				RenderContext::CreateDesc renderContextDesc;
				renderContextDesc.window = this;
				_renderContext = renderer->createContext(renderContextDesc);
			}

			_camera.setPos(0, 5, 5);
			_camera.setAim(0, 0, 0);
			
			EditMesh editMesh;
			WavefrontObjLoader::loadFile(editMesh, "bunny.obj");
			for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
				editMesh.color.emplace_back(255, 255, 255, 255);
			}

			// the current shader has no uv
			editMesh.uv[0].clear();

			_renderMesh.create(editMesh);
			auto shader = renderer->createShader("Shaders/test.shader");
			_material = Renderer::current()->createMaterial();
			_material->setShader(shader);
		}

		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}

		virtual void onDraw() {
			Base::onDraw();
			if (!_renderContext || !_material) return;

			_camera.setViewport(viewRect);

			{
				auto model = Mat4f::s_identity();
				auto view = _camera.viewMatrix();
				auto proj = _camera.projMatrix();
				auto mvp = proj * view * model;

				_material->setParam("sge_matrix_model", model);
				_material->setParam("sge_matrix_view", view);
				_material->setParam("sge_matrix_proj", proj);
				_material->setParam("sge_matrix_mvp", mvp);

				_material->setParam("sge_camera_pos", _camera.pos());

				_material->setParam("sge_light_pos", Vec3f(10, 10, 0));
				_material->setParam("sge_light_dir", Vec3f(-5, -10, -2));
				_material->setParam("sge_light_power", 4.0f);
				_material->setParam("sge_light_color", Vec3f(1, 1, 1));
			}

			auto s = 1.0f;

			_material->setParam("test_float", s * 0.5f);
			_material->setParam("test_color", Color4f(s, s, s, 1));

			_renderContext->setFrameBufferSize(viewRect.size);
			_renderContext->beginRender();

			_cmdBuf.reset();
			_cmdBuf.clearFrameBuffers()->setColor({ 0, 0, 0.2f, 1 });
			_cmdBuf.drawMesh(SGE_LOC, _renderMesh, _material);
			_cmdBuf.swapBuffers();

			_renderContext->commit(_cmdBuf);

			_renderContext->endRender();
			drawNeeded();
		}
		RenderMesh _renderMesh;
		RenderCommandBuffer _cmdBuf;
		SPtr<Material> _material;
		SPtr<RenderContext>	_renderContext;
		Math::Camera3f	_camera;
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