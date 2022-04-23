#pragma once

#include <RenderCommon.h>
#include <buffer/RenderGpuBuffer.h>
#include "VertexLayout.h"
#include "EditMesh.h"

namespace SimpleGameEngine {
	class RenderMesh {
	public:
		void create(const EditMesh& mesh);

		RenderGpuBuffer* vertexBuf() { return _vertexBuf; }
		RenderGpuBuffer* indexBuf() { return _indexBuf; }
		size_t vertexCount() const { return _vertexCount; }
		const VertexLayout* layout() const { return _layout; }
		RenderPrimitiveType primitive() const { return _primitive; }

	private:
		const VertexLayout*	_layout = nullptr;

		RenderPrimitiveType _primitive = RenderPrimitiveType::None;

		size_t _vertexCount = 0;
		SPtr<RenderGpuBuffer> _vertexBuf;
		SPtr<RenderGpuBuffer> _indexBuf;
	};
}