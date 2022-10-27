#pragma once

#include "../Component.h"
#include <sgerender.h>

namespace SimpleGameEngine {
	class CRenderer : public Component {
		SGE_OBJECT_TYPE(CRenderer, Component)
	public:
		Material* material() { return _material; }
		void setMaterial(Material* _mat) { _material = _mat; }

		virtual void render(RenderRequest& _renderRequest) = 0;
	protected:
		Material* _material;
	};

}