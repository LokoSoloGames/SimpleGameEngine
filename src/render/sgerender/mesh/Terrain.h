#include <render-pch.h>
#include <sgerender/buffer/RenderGpuBuffer.h>
#include "EditMesh.h"

namespace SimpleGameEngine {
	struct TerrainCreateDesc {
		Vec2i wh;
		// Texture?
		// Height Map?
	};

	class Terrain {
	public:
		void create(const TerrainCreateDesc& desc);
		void setAdjacentLOD(u8 mask);

		RenderGpuBuffer* vertexBuffer() const { return constCast(_vertexBuffer); }
		RenderGpuBuffer* indexBuffer()  const { return constCast(_indexBuffer); }

		size_t vertexCount() const { return pos.size(); }
		size_t indexCount() const { return indices.size(); }
		RenderDataType indexType() const { return _indexType; }

	private:
		Vector<Tuple3f>	pos;
		Vector<u32>	indices;
		Vector<Color4b>	colors;

		RenderDataType _indexType = RenderDataType::None;

		SPtr<RenderGpuBuffer>	_vertexBuffer;
		SPtr<RenderGpuBuffer>	_indexBuffer;

		const VertexLayout* _vertexLayout = nullptr;
	};

	struct TerrainCreateDesc {
		Vec2i wh;

	};
}