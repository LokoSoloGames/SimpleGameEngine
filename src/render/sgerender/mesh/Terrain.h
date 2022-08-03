#pragma once

#include <render-pch.h>
#include <sgerender/buffer/RenderGpuBuffer.h>
#include "EditMesh.h"

namespace SimpleGameEngine {
	class Terrain;

	class SubTerrain {
	public :
		void create(const Vec2i wh, const Vec3f offset);
		void setLod();
		void clear();

		RenderGpuBuffer* vertexBuffer() const { return constCast(_vertexBuffer); }
		RenderGpuBuffer* indexBuffer()  const { return constCast(_indexBuffer); }

		size_t vertexCount() const { return _vertexCount; }
		size_t indexCount() const { return _indexCount; }
		RenderDataType indexType() const { return _indexType; }

		RenderPrimitiveType primitive() const;
		const VertexLayout* vertexLayout() const;

		friend class Terrain;
	protected:
		Terrain* _terrain = nullptr;
		RenderDataType _indexType = RenderDataType::None;

		SPtr<RenderGpuBuffer>	_vertexBuffer;
		SPtr<RenderGpuBuffer>	_indexBuffer;

		Vector<u32>	indices;
		size_t _vertexCount = 0;
		size_t _indexCount = 0;
	};

	struct TerrainCreateDesc {
		Vec2i subTerrainSize;
		Vec2i subTerrainCount;
		// Texture?
		// Height Map?
	};

	class Terrain {
	public:
		void create(const TerrainCreateDesc& desc);
		void setSubTerrainCount(size_t newSize);

		RenderPrimitiveType primitive() const {
			return RenderPrimitiveType::Triangles;
		}
		const VertexLayout* vertexLayout() const {
			return _vertexLayout;
		}

		Span<      SubTerrain>	subTerrains()		{ return _subTerrains; }
		Span<const SubTerrain>	subTerrains() const	{ return _subTerrains; }
	private:
		const VertexLayout* _vertexLayout = nullptr;
		Vector<SubTerrain, 1>	_subTerrains;
	};

	SGE_INLINE RenderPrimitiveType SubTerrain::primitive() const {
		return _terrain->primitive();
	}

	SGE_INLINE const VertexLayout* SubTerrain::vertexLayout() const {
		return _terrain->vertexLayout();
	}
}