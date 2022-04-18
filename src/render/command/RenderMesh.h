#pragma once

#include "VertexLayout.h"
#include "Mesh.h"

namespace SimpleGameEngine {
	class RenderMesh {
	public:
		RenderMesh(Mesh& mesh);
		~RenderMesh();

		VertexLayout* layout = nullptr;
		void* vertexBuffer = nullptr;
		size_t vertexStride = 0;
		void* indexBuffer = nullptr;
		u32 count = 0;
	};


}