#include "RenderMesh.h"
#include <Renderer.h>
#include <buffer/RenderGpuBuffer.h>
#include <sgecore/error/Error.h>

namespace SimpleGameEngine {
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

		_primitive = RenderPrimitiveType::Triangles;
		_vertexCount = mesh.pos.size();
		if (_vertexCount <= 0)
			return;

		if (mesh.color.size() > 0) {
			if (mesh.color.size() < _vertexCount) {
				SGE_ASSERT(false);
			} else {
				colorCount = 1;
			}
		}

		for (size_t i = 0; i < EditMesh::kUvCountMax; i++) {
			auto& uv = mesh.uv[i];
			if (uv.size() > 0) {
				if (uv.size() <= _vertexCount) {
					SGE_ASSERT(false);
				} else {
					uvCount = i + 1;
				}
			}
		}

		_layout = &VertexLayout::PosColor; // TEMP
		Vector_<u8, 1024> vertexData;
		vertexData.resize(_layout->stride * _vertexCount);
		{
			auto* data = vertexData.data();
			auto stride = _layout->stride;

			for (auto& element : _layout->elements) {
				switch (element.semantic) {
					case VertexSemantic::POSITION: {
						RenderMesh_copyVertexData(data, _vertexCount, element, stride, mesh.pos.data());
					} break;
					case VertexSemantic::COLOR: {
						RenderMesh_copyVertexData(data, _vertexCount, element, stride, mesh.color.data());
					} break;
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