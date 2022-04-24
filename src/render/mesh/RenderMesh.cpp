#include "RenderMesh.h"
#include <Renderer.h>
#include <vertex/VertexLayoutManager.h>

namespace SimpleGameEngine {
	struct RenderMesh_InternalHelper {
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

	template<class DST, class T> inline
	void RenderMesh_copyVertexData(DST* dst, size_t vertexCount, const VertexLayout::Element& element, size_t stride, const T* src) {
		u8* p = dst + element.offset;

		for (size_t i = 0; i < vertexCount; i++) {
			*reinterpret_cast<T*>(p) = *src;
			src++;
			p += stride;
		}
	}

	void RenderMesh::create(const EditMesh& mesh) {
		u8 uvCount = 0;
		u8 colorCount = 0;
		u8 normalCount = 0;
		u8 tangentCount = 0;
		u8 binormalCount = 0;
		using Helper = RenderMesh_InternalHelper;

		_primitive = RenderPrimitiveType::Triangles;
		_vertexCount = mesh.pos.size();
		if (_vertexCount <= 0)
			return;

		if (Helper::hasAttr(mesh.color.size(),		_vertexCount)) colorCount = 1;
		if (Helper::hasAttr(mesh.normal.size(),		_vertexCount)) normalCount = 1;
		if (Helper::hasAttr(mesh.tangent.size(),	_vertexCount)) tangentCount = 1;
		if (Helper::hasAttr(mesh.binormal.size(),	_vertexCount)) binormalCount = 1;

		for (u8 i = 0; i < EditMesh::kUvCountMax; i++) {
			if (Helper::hasAttr(mesh.uv[i].size(), _vertexCount)) uvCount = i + 1;
		}

		auto vertexType = VertexTypeUtil::make(
			RenderDataTypeUtil::get<Tuple3f>(),
			RenderDataTypeUtil::get<Color4b>(), colorCount,
			RenderDataTypeUtil::get<Tuple2f>(), uvCount,
			RenderDataTypeUtil::get<Tuple3f>(), normalCount, tangentCount, binormalCount);

		_layout = VertexLayoutManager::current()->getLayout(vertexType);
		if (!_layout) {
			throw SGE_ERROR("cannot find vertex layout for mesh");
		}

		Vector_<u8, 1024> vertexData;
		vertexData.resize(_layout->stride * _vertexCount);
		{
			auto* pData = vertexData.data();
			auto stride = _layout->stride;

			for (auto& element : _layout->elements) {
				auto semanticType = Vertex_SemanticUtil::getType(element.semantic);
				auto semanticIndex = Vertex_SemanticUtil::getIndex(element.semantic);

				switch (semanticType) {
				case Vertex_SemanticType::TexCoord: {
					if (semanticIndex < EditMesh::kUvCountMax) {
						Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.uv[semanticIndex].data()); break;
					}
					continue;
				} break;
				}

				switch (element.semantic) {
					case Vertex_Semantic::Pos:		Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.pos.data());		break;
					case Vertex_Semantic::Color0:	Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.color.data());	break;
					case Vertex_Semantic::Normal:	Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.normal.data());	break;
					case Vertex_Semantic::Tangent:	Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.tangent.data());	break;
					case Vertex_Semantic::Binormal:	Helper::copyVertexData(pData, _vertexCount, element, stride, mesh.binormal.data()); break;
				}
			}
		}

		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Vertex;
		desc.bufferSize = _vertexCount * _layout->stride;
		_vertexBuf.reset(Renderer::current()->createGpuBuffer(desc));
		_vertexBuf->uploadToGpu(vertexData);
	}
}