#pragma once

#include <sgerender.h>
#include <sgerender/mesh/RenderMesh.h>
#include <sgerender/material/Material.h>

namespace SimpleGameEngine {
	class RenderCommand_Draw : NonCopyable {
	public:
		RenderPrimitiveType		primitive = RenderPrimitiveType::None;
		const VertexLayout* vertexLayout = nullptr;
		RenderDataType			indexType = RenderDataType::UInt16;

		SPtr<RenderGpuBuffer>	vertexBuffer;
		SPtr<RenderGpuBuffer>	indexBuffer;

		SPtr<MaterialPass>		materialPass;

		size_t vertexCount = 0;
		size_t indexCount = 0;
	};
}