#include "DirectX11_RenderGpuBuffer.h"
#include "DirectX11_Renderer.h"
#include <sgecore/math/Math.h>

namespace SimpleGameEngine {
	DirectX11_RenderGpuBuffer::DirectX11_RenderGpuBuffer(CreateDesc& desc): Base(desc) {
		if (desc.bufferSize <= 0) throw SGE_ERROR("buffer size = 0");
		if (desc.stride <= 0) throw SGE_ERROR("stride == 0");

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = Util::castUINT(Math::alignTo(desc.bufferSize, 16));
		bd.StructureByteStride = Util::castUINT(desc.stride);

		switch (desc.type) {
			case Type::Vertex: {
				bd.Usage			= D3D11_USAGE_DYNAMIC;		// write access access by CPU and GPU
				bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	// specify buffer type
				bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;	// allow CPU to write in buffer
			}break;
			case Type::Index: {
				bd.Usage			= D3D11_USAGE_DYNAMIC;
				bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			}break;
			case Type::Const: {
				bd.Usage			= D3D11_USAGE_DYNAMIC;
				bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
				bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			}break;
			default: throw SGE_ERROR("unsupported gpu buffer type");
		}

		auto* renderer = DirectX11_Renderer::current();
		auto* dev = renderer->d3dDevice();

		auto hr = dev->CreateBuffer(&bd, nullptr, _d3dBuf.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_RenderGpuBuffer::onUploadToGpu(Span<const u8> data, size_t offset) {
		auto* renderer = DirectX11_Renderer::current();
		auto* ctx = renderer->d3dDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mapped = {};

		auto hr = ctx->Map(_d3dBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		Util::throwIfError(hr);

		u8* dst = reinterpret_cast<u8*>(mapped.pData);
		memcpy(dst + offset, data.data(), data.size());

		ctx->Unmap(_d3dBuf, 0);
	}
}