#include "RenderTerrain.h"

namespace SimpleGameEngine {
	void RenderTerrain::createFromHeightMapFile(const CreateDesc& desc, StrView heightMapFileName) {
		Image img;
		img.loadPngFile(heightMapFileName);
		createFromHeightMap(desc, img);
	}

	void RenderTerrain::createFromHeightMap(const CreateDesc& desc, const Image& heightMap) {
		auto vertexType = VertexTypeUtil::make(
			RenderDataTypeUtil::get<Tuple3f>(),
			RenderDataTypeUtil::get<Color4b>(), 1,
			RenderDataTypeUtil::get<Tuple2f>(), 0,
			RenderDataTypeUtil::get<Tuple3f>(), 0, 0, 0);

		_vertexLayout = VertexLayoutManager::instance()->getLayout(vertexType);
		if (!_vertexLayout) {
			throw SGE_ERROR("cannot find vertex Layout for terrain");
		}
		setSubTerrainCount(desc.subTerrainCount.x * desc.subTerrainCount.y);
		auto* p = _subTerrains.data();
		for (int y = 0; y < desc.subTerrainCount.y; y++) {
			for (int x = 0; x < desc.subTerrainCount.x; x++) {
				Vec3f offset(static_cast<float>(desc.subTerrainSize.x * x), 0, static_cast<float>(desc.subTerrainSize.y * y));
				p->create(desc.subTerrainSize, offset);
				p++;
			}
		}
	}

	void RenderTerrain::setSubTerrainCount(size_t newSize) {
		size_t oldSize = _subTerrains.size();
		_subTerrains.resize(newSize);
		for (size_t i = oldSize; i < newSize; i++) {
			_subTerrains[i]._terrain = this;
		}
	}

	void Patch::create(const Vec2i wh, const Vec3f offset) {
		using Helper = Terrain_InternalHelper;
		_vertexCount = (wh.x + 1) * (wh.y + 1);
		
		Vector<Tuple3f> pos;
		Vector<Color4b> colors;
		pos.reserve(_vertexCount);
		colors.reserve(_vertexCount);
		auto halfWh = Vec2f(static_cast<float>(wh.x) / 2, static_cast<float>(wh.y) / 2);
		for (float y = -halfWh.y; y <= halfWh.y; y += 1) {
			for (float x = -halfWh.x; x <= halfWh.x; x += 1) {
				pos.emplace_back(Vec3f(x, 0, y) + offset);
				colors.emplace_back(255, 255, 255, 255);
			}
		}

		// set basic indices
		int lod = 1;
		int step = static_cast<int>(std::pow(2, lod));
		int columnStep = (wh.x + 1) * step;
		indices.reserve(wh.x * wh.y * 6 / (step * step));
		for (int y = 0; y < wh.y; y += step) {
			for (int x = 0; x < wh.x; x += step) {
				int currentIdx = y * (wh.x + 1) + x;
				if (currentIdx + columnStep + step >= _vertexCount) continue;
				indices.emplace_back(currentIdx);
				indices.emplace_back(currentIdx + columnStep);
				indices.emplace_back(currentIdx + columnStep + step);
				indices.emplace_back(currentIdx);
				indices.emplace_back(currentIdx + columnStep + step);
				indices.emplace_back(currentIdx + step);
			}
		}
		_indexCount = indices.size();

		auto* vertexLayout = _terrain->vertexLayout();

		Vector<u8, 1024>	vertexData;
		vertexData.resize(vertexLayout->stride * _vertexCount);

		auto* pData = vertexData.data();
		auto stride = vertexLayout->stride;
		auto vc = _vertexCount;

		for (auto& e : vertexLayout->elements) {
			using S = VertexSemantic;
			using ST = VertexSemanticType;
			using U = VertexSemanticUtil;

			/*auto semanticType = U::getType(e.semantic);
			auto semanticIndex = U::getIndex(e.semantic);*/

			switch (e.semantic) {
			case S::POSITION:	Helper::copyVertexData(pData, vc, e, stride, pos.data());   break;
			case S::COLOR0:		Helper::copyVertexData(pData, vc, e, stride, colors.data()); break;
			}
		}

		auto* renderer = Renderer::instance();
		{
			RenderGpuBuffer::CreateDesc _desc;
			_desc.type = RenderGpuBufferType::Vertex;
			_desc.bufferSize = _vertexCount * vertexLayout->stride;
			_vertexBuffer = renderer->createGpuBuffer(_desc);
			_vertexBuffer->uploadToGpu(vertexData);
		}
		{
			ByteSpan indexData;
			Vector<u16, 1024> index16Data;

			if (_vertexCount > UINT16_MAX) {
				_indexType = RenderDataType::UInt32;
				indexData = spanCast<const u8, const u32>(indices);
			}
			else {
				_indexType = RenderDataType::UInt16;
				index16Data.resize(indices.size());
				for (size_t i = 0; i < indices.size(); i++) {
					u32 vi = indices[i];
					index16Data[i] = static_cast<u16>(vi);
				}
				indexData = spanCast<const u8, const u16>(index16Data);
			}

			RenderGpuBuffer::CreateDesc _desc;
			_desc.type = RenderGpuBufferType::Index;
			_desc.bufferSize = indexData.size();
			_indexBuffer = renderer->createGpuBuffer(_desc);
			_indexBuffer->uploadToGpu(indexData);
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

	void RenderTerrain::render(RenderRequest& req) {
		for (auto& p : _patches) {
			p.calculateDisplayLevel(req.camera_pos);
		}

		for (auto& p : _patches) {
			p.render(req);
		}
	}

	inline
	bool RenderTerrain::Patch::_adjacentPatchHasHigherLod(int x, int y) const {
		auto* adj = _terrain->patch(_index.x + x, _index.y + y);
		if (!adj) return false;
		return adj->displayLevel() > _displayLevel;
	}

	void RenderTerrain::Patch::calculateDisplayLevel(const Vec3f& viewPos) {
		auto distance = (worldCenterPos() - viewPos).length();
		auto d = _terrain->patchSize().x * 5;
		_displayLevel = static_cast<int>(distance / d);
	}
}