#include "RenderGpuBuffer.h"

namespace SimpleGameEngine {
	RenderGpuBuffer::RenderGpuBuffer(CreateDesc& desc): _desc(desc) { }

	void RenderGpuBuffer::uploadToGpu(ByteSpan data, size_t offset /*= 0*/) {
		if (data.size() + offset > _desc.bufferSize) {
			throw SGE_ERROR("out of range");
		}
		onUploadToGpu(data, offset);
	}
}