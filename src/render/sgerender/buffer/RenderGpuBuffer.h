#pragma once

#include <render-pch.h>

namespace SimpleGameEngine {
	#define RenderGpuBufferType_ENUM_LIST(E) \
		E(None,) \
		E(Vertex,) \
		E(Index,) \
		E(Const,) \
	//----
	SGE_ENUM_CLASS(RenderGpuBufferType, u8)

	struct RenderGpuBufferCreateDesc {
		using Type = RenderGpuBufferType;

		Type	type = Type::None;
		size_t	bufferSize = 0;
		size_t	stride = 16;
	};

	class RenderGpuBuffer : public RefCountBase {
	public:
		using CreateDesc = RenderGpuBufferCreateDesc;

		RenderGpuBuffer(CreateDesc& desc);

		void uploadToGpu(ByteSpan data, size_t offset = 0);
		size_t bufferSize() const { return _desc.bufferSize; }

		const CreateDesc& desc() const { return _desc; }
	protected:
		virtual void onUploadToGpu(ByteSpan data, size_t offset) = 0;
		CreateDesc _desc;
	};
}