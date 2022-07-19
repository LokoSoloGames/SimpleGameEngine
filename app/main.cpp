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

			auto* renderer = Renderer::instance();
			{
				RenderContext::CreateDesc renderContextDesc;
				renderContextDesc.window = this;
				_renderContext = renderer->createContext(renderContextDesc);
			}

			_camera.setPos(0, 5, 5);
			_camera.setAim(0, 0, 0);

			{
				Texture2D_CreateDesc texDesc;
				auto& image = texDesc.imageToUpload;

				image.loadFile("Textures/uvChecker_BC7.dds");

				texDesc.size = image.size();
				texDesc.colorType = image.colorType();

				/*int w = 256;
				int h = 256;

				texDesc.size.set(w, h);
				texDesc.colorType = ColorType::RGBAb;

				image.create(Color4b::kColorType, w, h);

				for (int y = 0; y < w; y++) {
					auto span = image.row<Color4b>(y);
					for (int x = 0; x < h; x++) {
						span[x] = Color4b(static_cast<u8>(x),
							static_cast<u8>(y),
							0,
							255);
					}
				}*/

				_testTexture = renderer->createTexture2D(texDesc);
			}
			
			EditMesh editMesh;
			WavefrontObjLoader::loadFile(editMesh, "Mesh/test.obj");
			for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
				editMesh.color.emplace_back(255, 255, 255, 255);
			}

			_renderMesh.create(editMesh);
			auto shader = renderer->createShader("Shaders/test.shader");
			_material = Renderer::instance()->createMaterial();
			_material->setShader(shader);
			_material->setParam("mainTex", _testTexture);

		}

		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}

		virtual void onUIMouseEvent(UIMouseEvent& ev) override {
			if (ev.isDragging()) {
				using Button = UIMouseEventButton;
				switch (ev.pressedButtons) {
					case Button::Left: {
						auto d = ev.deltaPos * 0.01f;
						_camera.orbit(d.x, d.y);
					}break;

					case Button::Middle: {
						auto d = ev.deltaPos * 0.005f;
						_camera.move(d.x, d.y, 0);
					}break;

					case Button::Right: {
						auto d = ev.deltaPos * -0.005f;
						_camera.dolly(d.x + d.y);
					}break;
				}
			}
		}

		virtual void onDraw() {
			Base::onDraw();
			if (!_renderContext || !_material) return;

			_camera.setViewport(clientRect());

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

			_renderContext->setFrameBufferSize(clientRect().size);
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
		SPtr<Texture2D>	_testTexture;
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