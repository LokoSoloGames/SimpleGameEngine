#pragma once

#include <sgebase.h>

namespace SimpleGameEngine {
	enum class RenderGpuBufferType {
		None,
		Vertex,
		Index,
		Const,
	};

	struct RenderGpuBufferCreateDesc {
		using Type = RenderGpuBufferType;

		Type	type = Type::None;
		size_t	bufferSize = 0;
		size_t	stride = 16;
	};

	class RenderGpuBuffer : public Object {
	public:
		using CreateDesc = RenderGpuBufferCreateDesc;

		RenderGpuBuffer(CreateDesc& desc);

		void uploadToGpu(ByteSpan data, size_t offset = 0) {
			if (data.size() + offset > _desc.bufferSize) {
				throw SGE_ERROR("out of range");
			}
			onUploadToGpu(data, offset);
		}

	protected:
		virtual void onUploadToGpu(ByteSpan data, size_t offset) = 0;
		CreateDesc _desc;
	};
}