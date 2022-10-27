#pragma once

#include "CRenderer.h"

namespace SimpleGameEngine {

	class CMeshRenderer : public CRenderer {
		SGE_OBJECT_TYPE(CMeshRenderer, CRenderer)
	public:
		void setMesh(const EditMesh& mesh);

		virtual void render(RenderRequest& _renderRequest) override;
	protected:
		RenderMesh _renderMesh;
	};

}