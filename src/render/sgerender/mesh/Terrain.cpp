#include "Terrain.h"
#include <sgerender/Renderer.h>
#include <sgerender/vertex/VertexLayoutManager.h>

namespace SimpleGameEngine {
	struct Terrain_InternalHelper {
		template<class DST, class T> inline
			static void copyVertexData(DST* dst
				, size_t vertexCount
				, const VertexLayout::Element& element
				, size_t stride
				, const T* src)
		{
			u8* p = dst + element.offset;

			for (size_t i = 0; i < vertexCount; i++) {
				*reinterpret_cast<T*>(p) = *src;
				src++;
				p += stride;
			}
		}

		static bool hasAttr(size_t arraySize, size_t vertexCount) {
			if (arraySize <= 0) return false;
			if (arraySize < vertexCount) {
				SGE_ASSERT(false);
				return false;
			}
			return true;
		}
	};

	void Terrain::create(const TerrainCreateDesc& desc) {
		using Helper = Terrain_InternalHelper;

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

		Vector_<u8, 1024>	vertexData;
		vertexData.resize(_vertexLayout->stride * size);

		auto* pData = vertexData.data();
		auto stride = _vertexLayout->stride;
		auto vc = size;

		for (auto& e : _vertexLayout->elements) {
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
			_desc.bufferSize = size * _vertexLayout->stride;
			_vertexBuffer = renderer->createGpuBuffer(_desc);
			_vertexBuffer->uploadToGpu(vertexData);
		}
		{
			ByteSpan indexData;
			Vector_<u16, 1024> index16Data;

			if (size > UINT16_MAX) {
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
}