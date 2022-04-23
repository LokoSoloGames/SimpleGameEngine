#pragma once

#include <sgebase.h>
#include <sgecore/error/Error.h>

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

		using Type = RenderGpuBufferType;
		using CreateDesc = RenderGpuBufferCreateDesc;

		void uploadToGpu(Span<const u8> data, size_t offset = 0) {
			if (data.size() + offset > _desc.bufferSize) {
				throw SGE_ERROR("out of range");
			}
			onUploadToGpu(data, offset);
		}

	protected:
		virtual void onUploadToGpu(Span<const u8> data, size_t offset) = 0;
		CreateDesc _desc;
	};
}