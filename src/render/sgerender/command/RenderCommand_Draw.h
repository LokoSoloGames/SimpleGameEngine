#pragma once

#include <sgerender.h>
#include <sgerender/mesh/RenderMesh.h>
#include <sgerender/material/Material.h>

namespace SimpleGameEngine {
	class RenderCommand_Draw {
	public:
		RenderSubMesh* renderMesh;
		Material* material;
	};
}