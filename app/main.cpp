#include <nativeui/NativeUI.h>
#include <sgecore.h>
#include <sgerender.h>
#include <sgeeditor.h>

namespace SimpleGameEngine {

	class MainWin : public NativeUIWindow {
		using Base = NativeUIWindow;
	public:
		void onCreate(CreateDesc& desc) {
			Base::onCreate(desc);

			auto* renderer = Renderer::instance();
			auto* editor = EditorContext::instance();
			{
				RenderContext::CreateDesc renderContextDesc;
				renderContextDesc.window = this;
				_renderContext = renderer->createContext(renderContextDesc);
			}

			_camera.setPos(0, 10, 10);
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
			{
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
			{
				float size = 2048;
				float pos = size / -2;
				float y = -100;
				float height = 200;
				int maxLod = 7;
				RenderTerrainCreateDesc _desc;
				_desc.terrainPos = Vec3f(pos, y, pos);
				_desc.terrainSize = Vec2f(size, size);
				_desc.terrainHeight = height;
				_desc.maxLod = maxLod;
				_terrain.createFromHeightMapFile(_desc, "Terrain/TerrainTest/TerrainHeight_Small.png");
			}

			{ // ECS
				for (int i = 0; i < 10; i++) {
					auto* e = _scene.addEntity("Object 1");
					auto* t = e->addComponent<CTransform>();
					t->position.set(static_cast<float>(i), 5, 10);
				}

				editor->entitySelection.add(EntityId(1));
				editor->entitySelection.add(EntityId(3));
			}
		}

		virtual void onCloseButton() override {
			NativeUIApp::current()->quit(0);
		}

		virtual void onUIMouseEvent(UIMouseEvent& ev) override {
			_renderContext->onUIMouseEvent(ev);

			if (ev.isDragging()) {
				using Button = UIMouseEventButton;
				switch (ev.pressedButtons) {
					case Button::Left: {
						auto d = ev.deltaPos * 0.01f;
						_camera.orbit(d.x, d.y);
					}break;

					case Button::Middle: {
						auto d = ev.deltaPos * 0.1f;
						_camera.move(d.x, d.y, 0);
					}break;

					case Button::Right: {
						auto d = ev.deltaPos * -0.1f;
						_camera.dolly(d.x + d.y);
					}break;
				}
			}
		}

		virtual void onDraw() {
			Base::onDraw();
			if (!_renderContext || !_material) return;

			_camera.setViewport(clientRect());

			_renderContext->setFrameBufferSize(clientRect().size);
			_renderContext->beginRender();

			_renderRequest.reset(_renderContext);
			_renderRequest.matrix_model = Mat4f::s_identity();
			_renderRequest.matrix_view = _camera.viewMatrix();
			_renderRequest.matrix_proj = _camera.projMatrix();
			_renderRequest.camera_pos = _camera.pos();

			_renderRequest.clearFrameBuffers()->setColor({ 0, 0, 0.2f, 1 });

			auto s = 1.0f;

			_material->setParam("test_float", s * 0.5f);
			_material->setParam("test_color", Color4f(s, s, s, 1));
//------
			_renderRequest.drawMesh(SGE_LOC, _renderMesh, _material);

			_terrain.render(_renderRequest);

			_hierarchyWindow.draw(_renderRequest, _scene);
			_inspectorWindow.draw(_renderRequest, _scene);

			_renderContext->drawUI(_renderRequest);
			_renderRequest.swapBuffers();

			_renderContext->commit(_renderRequest.commandBuffer);

			_renderContext->endRender();
			drawNeeded();
		}
		RenderMesh _renderMesh;
		SPtr<Material> _material;
		SPtr<Texture2D>	_testTexture;
		SPtr<RenderContext>	_renderContext;
		RenderTerrain _terrain;

		Math::Camera3f	_camera;
		Scene			_scene;
		RenderRequest	_renderRequest;

		EditorHierarchyWindow		_hierarchyWindow;
		EditorInspectorWindow		_inspectorWindow;
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
			EditorContext::createContext();

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

		virtual void onQuit() {
			EditorContext::destroyContext();
			Base::onQuit();
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