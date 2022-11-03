#include "CMeshRenderer.h"
#include <sgeengine/ECS/Entity.h>

namespace SimpleGameEngine {
	const TypeInfo* CMeshRenderer::s_getType() {
		class TI : public TI_Base {
		public:
			TI() {
				//static FieldInfo fi[] = {
				//};
				//setFields(fi);
			}
		};
		static TI ti;
		return &ti;
	}

	void CMeshRenderer::setMesh(const EditMesh& mesh) {
		_renderMesh.clear();
		_renderMesh.create(mesh);
	}

	void CMeshRenderer::render(RenderRequest& _renderRequest) {
		if (_renderMesh.subMeshes().size() <= 0) return;
		if (!_material) {
			SGE_LOG_WARN("Mesh Renderer Component Missing Material");
			return;
		}

		_renderRequest.matrix_model = entity()->transform()->worldMatrix();
		_renderRequest.drawMesh(SGE_LOC, _renderMesh, _material);
		_renderRequest.matrix_model = Mat4f::s_identity(); // temp reset
	}
}