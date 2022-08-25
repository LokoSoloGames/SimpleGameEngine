#pragma once

#include "RenderMesh.h"
#include "../command/RenderRequest.h"

namespace SimpleGameEngine {
#define RenderTerrain3_ZoneMask_ENUM_LIST(E) \
	E(None, = 0) \
	E(North, = 1 << 0)  \
	E(East,  = 1 << 1)  \
	E(South, = 1 << 2)  \
	E(West,  = 1 << 3)  \
//----
	SGE_ENUM_CLASS(RenderTerrain3_ZoneMask, u8);
	SGE_ENUM_ALL_OPERATOR(RenderTerrain3_ZoneMask)

	struct RenderTerrainCreateDesc {
	public:
		Vec3f terrainPos;
		Vec2f terrainSize;
		float terrainHeight;
		int maxLod;
	};

	class RenderTerrain : NonCopyable {
		using This = RenderTerrain;
	public:
		using Terrain		= This;
		using CreateDesc	= RenderTerrainCreateDesc;
		using ZoneMask		= RenderTerrain3_ZoneMask;
		using Vertex		= Vertex_Pos2f;
		using VertexIndex	= u16;

		void createFromHeightMapFile(const CreateDesc& desc, StrView		heightMapFileName);
		void createFromHeightMap	(const CreateDesc& desc, const Image&	heightMap);
		void destroy();

		void render(RenderRequest& req);

		class PatchIndices {
		public:
			void create(Terrain* terrain, int level, ZoneMask zoneMask);
			RenderGpuBuffer* indexBuffer() const { return constCast(_indexBuffer.ptr()); }
			size_t indexCount() const { return _indexCount; }
			RenderDataType indexType() const { return RenderDataTypeUtil::get<VertexIndex>(); }

		private:
			void _addToIndices(Vector<VertexIndex>& indices, Span<Vec2i> sector, int verticesPerRow, Vec2i direction, bool flipXY);

			SPtr<RenderGpuBuffer> _indexBuffer;
			size_t _indexCount = 0;
		};

		class Patch {
		public:
			void create(Terrain* terrain, const Vec2i& index, Shader* shader);
			int displayLevel() const { return _displayLevel; }
			Vec3f worldCenterPos();
			void calculateDisplayLevel(const Vec3f& viewPos);
			void render(RenderRequest& request);
			
		private:
			inline bool _adjacentPatchHasHigherLod(int x, int y) const;

			int				_displayLevel = 0;
			Vec2i			_index{ 0, 0 };
			RenderTerrain*  _terrain = nullptr;
			SPtr<Material>	_material;			
		};

		int		patchCellsPerRow()		const { return 1 << (_maxLod - 1); }
		int		patchVerticesPerRow()	const { return patchCellsPerRow() + 1; }
		int		maxLod()				const { return _maxLod; }

		const	Vec3f& terrainPos()		const { return _terrainPos; }
		const	Vec2f& terrainSize()	const { return _terrainSize; }
				float  terrainHeight()	const { return _terrainHeight; }

		Vec2i	patchCount()	const	{ return _patchCount; }
		Vec2f	patchSize()		const	{ return _terrainSize / Vec2f::s_cast(_patchCount); }
		Vec2f	cellSize()		const	{ return _terrainSize / Vec2f::s_cast(_heightmapResolution - 1); }

		PatchIndices* patchIndices(int level, ZoneMask zoneMask);
		Patch* patch(int x, int y);

		RenderPrimitiveType primitive() const { return RenderPrimitiveType::Triangles; }
		const VertexLayout* vertexLayout() const { return _vertexLayout;	}
		RenderGpuBuffer* vertexBuffer() const { return constCast(_vertexBuffer); }
		size_t vertexCount() const { return _vertexCount; }
		Texture2D* heightMapTexture() { return _heightMapTexture; }

	private:
		Vec3f	_terrainPos{ 0, 0, 0 };
		Vec2f	_terrainSize{ 0, 0 };
		float	_terrainHeight = 0;
		Vec2i	_heightmapResolution{ 0, 0 };
		int		_maxLod = 1;
		Vec2i	_patchCount{ 0, 0 };

		static const int k_patchTypeCount = 16;

		Vector<Patch>			_patches;
		Vector<PatchIndices>	_patchIndices;

		const VertexLayout*		_vertexLayout = nullptr;
		size_t					_vertexCount = 0;
		SPtr<RenderGpuBuffer>	_vertexBuffer;

		SPtr<Texture2D>			_heightMapTexture;
	};

	SGE_INLINE RenderTerrain::PatchIndices* RenderTerrain::patchIndices(int level, ZoneMask zoneMask) {
		int idx = level * k_patchTypeCount + static_cast<u8>(zoneMask);
		if (idx < 0 || idx >= _patchIndices.size()) {
			SGE_ASSERT(false);
			return nullptr;
		}

		return &_patchIndices[idx];
	}

	SGE_INLINE RenderTerrain::Patch* RenderTerrain::patch(int x, int y) {
		if (x < 0 || y < 0 || x >= _patchCount.x || y >= _patchCount.y)
			return nullptr;
		return &_patches[y * _patchCount.x + x];
	}

	SGE_INLINE Vec3f RenderTerrain::Patch::worldCenterPos() {
		auto s = _terrain->patchSize();
		auto pos = (Vec2f::s_cast(_index) + 0.5f) * s;
		auto o = _terrain->terrainPos() + Vec3f(pos.x, 0, pos.y);
		return o;
	}
}