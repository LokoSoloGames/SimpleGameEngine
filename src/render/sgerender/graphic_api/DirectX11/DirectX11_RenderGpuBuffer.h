#pragma once

#include "DirectX11_Common.h"
#include <sgerender/buffer/RenderGpuBuffer.h>

namespace SimpleGameEngine {

	class DirectX11_RenderGpuBuffer : public RenderGpuBuffer {
		using Base = RenderGpuBuffer;
		using Type = RenderGpuBufferType;
		using Util = DX11Util;
	public:
		DirectX11_RenderGpuBuffer(CreateDesc& desc);
		DX11_ID3DBuffer* getBuffer() { return _d3dBuf.ptr(); }

		virtual void onUploadToGpu(ByteSpan data, size_t offset) override;

	private:
		ComPtr<DX11_ID3DBuffer> _d3dBuf;
	};

}