#include "Terrain.h"
#include <sgerender/vertex/VertexLayoutManager.h>

namespace SimpleGameEngine {
	void Terrain::create(const TerrainCreateDesc& desc) {
		int size = (desc.wh.x + 1) * (desc.wh.y + 1);
		pos.reserve(size);
		colors.reserve(size);
		for (int y = 0; y <= desc.wh.y; y++) {
			for (int x = 0; x <= desc.wh.x; x++) {
				pos.emplace_back(x, 0, y);
				colors.emplace_back(255, 255, 255, 255);
			}
		}

		// set basic indices
		indices.reserve(desc.wh.x * desc.wh.y * 6);
		for (int y = 0; y < desc.wh.y; y++) {
			for (int x = 0; x < desc.wh.x; x++) {
				int currentIdx = y * (desc.wh.x + 1) + x;
				indices.emplace_back(currentIdx);
				indices.emplace_back(currentIdx + (desc.wh.x + 1));
				indices.emplace_back(currentIdx + (desc.wh.x + 1) + 1);
				indices.emplace_back(currentIdx);
				indices.emplace_back(currentIdx + (desc.wh.x + 1) + 1);
				indices.emplace_back(currentIdx + 1);
			}
		}

		auto vertexType = VertexTypeUtil::make(
			RenderDataTypeUtil::get<Tuple3f>(),
			RenderDataTypeUtil::get<Color4b>(), 1,
			RenderDataTypeUtil::get<Tuple2f>(), 0,
			RenderDataTypeUtil::get<Tuple3f>(), 0, 0, 0);

		_vertexLayout = VertexLayoutManager::instance()->getLayout(vertexType);
		if (!_vertexLayout) {
			throw SGE_ERROR("cannot find vertex Layout for mesh");
		}
	}
}