#include "RenderMesh.h"
#include <Renderer.h>

namespace SimpleGameEngine {
	RenderMesh::RenderMesh(Mesh& mesh) {
		layout = &VertexLayout::PosColor;
		count = mesh.getSize();
		Vertex_PosColor* vertices = new Vertex_PosColor[count];
		u32* indices = new u32[count];
		for (int i = 0; i < count; ++i) {
			vertices[i] = Vertex_PosColor{mesh.positions[i], mesh.colors[i]};
			indices[i] = (u32)i;
		}

		vertexStride = sizeof(Vertex_PosColor);
		size_t vertexSize = sizeof(Vertex_PosColor) * count;
		size_t indexSize = sizeof(u32) * count;
		Renderer::current()->createBuffer(vertices, vertexSize, vertexBuffer);
		Renderer::current()->createBuffer(indices, indexSize, indexBuffer);
		delete[] vertices;
		delete[] indices;
	}

	RenderMesh::~RenderMesh() {
		layout = nullptr;
		Renderer::current()->releaseBuffer(vertexBuffer);
		Renderer::current()->releaseBuffer(indexBuffer);
	}
}