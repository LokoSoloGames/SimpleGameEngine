#include "RenderTerrain.h"
#include <sgerender.h>

namespace SimpleGameEngine {
	void RenderTerrain::createFromHeightMapFile(const CreateDesc& desc, StrView heightMapFileName) {
		Image img;
		img.loadPngFile(heightMapFileName);
		createFromHeightMap(desc, img);
	}

	void RenderTerrain::createFromHeightMap(const CreateDesc& desc, const Image& heightMap) {
		destroy();

		if (heightMap.width() < 1 || heightMap.height() < 1)
			throw SGE_ERROR("invalid height map size");

		if (desc.maxLod > 8) // vertex index > uint16
			throw SGE_ERROR("reach max lod  limit");

		_heightmapResolution.set(heightMap.width(), heightMap.height());
		_terrainPos = desc.terrainPos;
		_terrainSize = desc.terrainSize;
		_terrainHeight = desc.terrainHeight;

		_maxLod = desc.maxLod < 1 ? 1 : desc.maxLod;
		_patchCount = (_heightmapResolution - 1) / patchCellsPerRow();

		{ // Patch Indices
			_patchIndices.resize(_maxLod * k_patchTypeCount);
			for (int lv = 0; lv < _maxLod; lv++) {
				auto zoneMask = ZoneMask::None;
				for (int i = 0; i < k_patchTypeCount; i++) {
					_patchIndices[lv * k_patchTypeCount + i].create(this, lv, zoneMask);
					zoneMask += 1;
				}
			}
		}

		auto* renderer = Renderer::instance();
		{ // Patches
			_patches.resize(_patchCount.x * _patchCount.y);
			auto shader = renderer->createShader("Shaders/terrain.shader");

			auto* p = _patches.begin();
			for (int y = 0; y < _patchCount.y; y++) {
				for (int x = 0; x < _patchCount.x; x++) {
					p->create(this, Vec2i(x, y), shader);
					p++;
				}
			}
			SGE_ASSERT(p == _patches.end());
		}

		_vertexLayout = Vertex::s_layout();
		{ // Vertex Buffer
			Vector<Vertex>	vertexData;
			int verticesPerRow = patchVerticesPerRow();
			int cellsPerRow = patchCellsPerRow();

			_vertexCount = verticesPerRow * verticesPerRow;
			vertexData.resize(_vertexCount);

			auto scale = patchSize();

			auto* dst = vertexData.begin();
			for (int y = 0; y < verticesPerRow; y++) {
				for (int x = 0; x < verticesPerRow; x++) {
					dst->pos = Vec2f::s_cast(Vec2i(x, y)) * scale / static_cast<float>(cellsPerRow);
					dst++;
				}
			}
			SGE_ASSERT(dst == vertexData.end());

			RenderGpuBuffer::CreateDesc _desc;
			_desc.type = RenderGpuBufferType::Vertex;
			_desc.bufferSize = _vertexCount * _vertexLayout->stride;
			_vertexBuffer = renderer->createGpuBuffer(_desc);
			_vertexBuffer->uploadToGpu(spanCast<u8>(vertexData.span()));
		}

		{ // Height Map Texture
			Texture2D_CreateDesc _desc;
			_desc.size = heightMap.size();
			_desc.colorType = heightMap.colorType();
			_desc.imageToUpload.copy(heightMap);
			_desc.samplerState.filter = TextureFilter::Point;
			_desc.samplerState.wrapU = TextureWrap::Clamp;
			_desc.samplerState.wrapV = TextureWrap::Clamp;

			_heightMapTexture = renderer->createTexture2D(_desc);
		}
	}

	void RenderTerrain::destroy() {
		_terrainSize.set(0, 0);
		_terrainHeight = 0;
		_heightmapResolution.set(0, 0);
		_maxLod = 1;
		_patchCount.set(0, 0);
		_patches.clear();
		_patchIndices.clear();

		_vertexBuffer = nullptr;
		_vertexLayout = nullptr;
		_vertexCount = 0;
	}

	void RenderTerrain::Patch::create(Terrain* terrain, const Vec2i& index, Shader* shader) {
		_terrain = terrain;
		_index = index;

		_material = Renderer::instance()->createMaterial();
		_material->setShader(shader);
	}

	void RenderTerrain::render(RenderRequest& req) {
		for (auto& p : _patches) {
			p.calculateDisplayLevel(req.camera_pos);
		}

		for (auto& p : _patches) {
			p.render(req);
		}
	}

	void RenderTerrain::Patch::render(RenderRequest& req) {
		auto zoneMask = ZoneMask::None;
		if (_adjacentPatchHasHigherLod(0, -1)) zoneMask |= ZoneMask::North;
		if (_adjacentPatchHasHigherLod(1, 0))  zoneMask |= ZoneMask::East;
		if (_adjacentPatchHasHigherLod(0, 1))  zoneMask |= ZoneMask::South;
		if (_adjacentPatchHasHigherLod(-1, 0)) zoneMask |= ZoneMask::West;

		auto lv = Math::clamp(_displayLevel, int(0), _terrain->maxLod() - 1);
		auto* pi = _terrain->patchIndices(lv, zoneMask);
		if (!pi) { SGE_ASSERT(false); return; }

		if (!_material) { SGE_ASSERT(false); return; }

		req.setMaterialCommonParams(_material);
		_material->setParam("terrainHeightMap", _terrain->heightMapTexture());
		_material->setParam("terrainPos", _terrain->terrainPos());
		_material->setParam("terrainSize", _terrain->terrainSize());
		_material->setParam("terrainHeight", _terrain->terrainHeight());
		_material->setParam("patchCellsPerRow", _terrain->patchCellsPerRow());
		_material->setParam("patchIndex", _index);
		_material->setParam("patchSize", _terrain->patchSize());

		auto passes = _material->passes();

		for (size_t i = 0; i < passes.size(); i++) {
			auto* cmd = req.commandBuffer.newCommand<RenderCommand_DrawCall>();
#if _DEBUG
			cmd->debugLoc = SGE_LOC;
#endif
			cmd->material = _material;
			cmd->materialPassIndex = i;

			cmd->primitive		= RenderPrimitiveType::Triangles;
			cmd->vertexLayout	= _terrain->vertexLayout();
			cmd->vertexBuffer	= _terrain->vertexBuffer();
			cmd->vertexCount	= _terrain->vertexCount();
			cmd->indexBuffer	= pi->indexBuffer();;
			cmd->indexType		= pi->indexType();
			cmd->indexCount		= pi->indexCount();
		}
	}

	inline
	bool RenderTerrain::Patch::_adjacentPatchHasHigherLod(int x, int y) const {
		auto* adj = _terrain->patch(_index.x + x, _index.y + y);
		if (!adj) return false;
		return adj->displayLevel() > _displayLevel;
	}

	void RenderTerrain::Patch::calculateDisplayLevel(const Vec3f& viewPos) {
		auto center = worldCenterPos();
		auto distance = (center - viewPos).length();
		auto d = _terrain->patchSize().x * 1;
		_displayLevel = static_cast<int>(distance / d);
	}

	void RenderTerrain::PatchIndices::create(Terrain* terrain, int level, ZoneMask zoneMask) {
		Vector<VertexIndex>	indexData;

		int verticesPerRow = terrain->patchVerticesPerRow();
		int rows = 1 << (terrain->maxLod() - 1 - level);
		int n = rows / 2;
		int step = 1 << level;

		if (rows == 1) {
			VertexIndex x0 = 0;
			VertexIndex x1 = static_cast<VertexIndex>(verticesPerRow - 1);
			VertexIndex y0 = 0;
			VertexIndex y1 = static_cast<VertexIndex>((verticesPerRow - 1) * verticesPerRow);

			indexData.resize(6);

			indexData[0] = x0 + y0;
			indexData[1] = x1 + y1;
			indexData[2] = x1 + y0;

			indexData[3] = x0 + y0;
			indexData[4] = x0 + y1;
			indexData[5] = x1 + y1;

		}
		else {
			Vector<Vec2i, 256> sector0; // lod 0
			Vector<Vec2i, 256> sector1; // lod 1

			for (int y = 0; y < n; y++) {
				int lastRow = (y == n - 1) ? 1 : 0;

				for (int x = 0; x <= y; x++) {
					Vec2i v[3];
					int odd = (x + y) % 2;

					v[0] = Vec2i(x, y) * step;
					v[1] = Vec2i(x + 1, y + 1 - odd) * step;
					v[2] = Vec2i(x, y + 1) * step;
					sector0.appendRange(v);

					if (lastRow) {
						v[2].x = (x - 1 + odd) * step;
					}
					sector1.appendRange(v);

					if (x == y) break; // drop last triangle in this row

					v[0] = Vec2i(x, y + odd) * step;
					v[1] = Vec2i(x + 1, y) * step;
					v[2] = Vec2i(x + 1, y + 1) * step;

					sector0.appendRange(v);

					if (!lastRow || !odd) { // drop even number triangle in last row
						sector1.appendRange(v);
					}
				}
			}

			{ // north
				auto& sector = enumHas(zoneMask, ZoneMask::North) ? sector1 : sector0;
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(1, -1), false);
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(-1, -1), false);
			}
			{ // east
				auto& sector = enumHas(zoneMask, ZoneMask::East) ? sector1 : sector0;
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(1, 1), true);
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(-1, 1), true);
			}
			{ // south
				auto& sector = enumHas(zoneMask, ZoneMask::South) ? sector1 : sector0;
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(1, 1), false);
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(-1, 1), false);
			}
			{ // west
				auto& sector = enumHas(zoneMask, ZoneMask::West) ? sector1 : sector0;
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(1, -1), true);
				_addToIndices(indexData, sector, verticesPerRow, Vec2i(-1, -1), true);
			}
		}

		{
			auto* renderer = Renderer::instance();

			auto byteSpan = spanCast<const u8>(indexData.span());

			RenderGpuBuffer::CreateDesc desc;
			desc.type = RenderGpuBufferType::Index;
			desc.bufferSize = byteSpan.size();

			_indexCount = indexData.size();

			_indexBuffer = renderer->createGpuBuffer(desc);
			_indexBuffer->uploadToGpu(byteSpan);
		}
	}

	void RenderTerrain::PatchIndices::_addToIndices(Vector<VertexIndex>& indexData, Span<Vec2i> sector, int verticesPerRow, Vec2i direction, bool flipXY) {
		auto w = verticesPerRow;
		Vec2i center(w / 2, w / 2);
		int verticesPerPatch = verticesPerRow * verticesPerRow;

		Span<VertexIndex>	dstSpan;
		{
			auto oldSize = indexData.size();
			auto newSize = oldSize + sector.size();
			indexData.resize(newSize);
			dstSpan = indexData.subspan(oldSize);
		}

		{
			auto* dst = dstSpan.begin();

			for (auto s : sector) {
				s = s * direction + center;
				if (flipXY) s = s.yx();
				SGE_ASSERT(s.x >= 0 && s.y >= 0);

				auto vi = static_cast<VertexIndex>(s.x + s.y * verticesPerRow);
				SGE_ASSERT(vi < verticesPerPatch);

				*dst = vi;
				dst++;
			}
			SGE_ASSERT(dst == dstSpan.end());
		}

		int rx = direction.x < 0 ? 1 : 0;
		int ry = direction.y < 0 ? 1 : 0;
		int rf = flipXY ? 0 : 1;

		if (rx ^ ry ^ rf) {
			auto* dst = dstSpan.begin();
			auto* end = dstSpan.end();
			for (; dst < end; dst += 3) {
				swap(dst[0], dst[1]);
			}
		}
	}
}