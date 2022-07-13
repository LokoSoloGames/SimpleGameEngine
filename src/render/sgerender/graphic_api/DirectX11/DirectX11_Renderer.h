#pragma once

#include "DirectX11_Common.h"

#if SGE_RENDER_HAS_DX11
namespace SimpleGameEngine {
	class DirectX11_Renderer : public Renderer {
		using Base = Renderer;
		using Util = DX11Util;
	public:
		static DirectX11_Renderer* instance() { return static_cast<DirectX11_Renderer*>(_instance); }

		DirectX11_Renderer(CreateDesc& desc);

		DX11_IDXGIFactory*		dxgiFactory()		{ return _dxgiFactory; }
		DX11_IDXGIDevice*		dxgiDevice()		{ return _dxgiDevice; }
		DX11_IDXGIAdapter*		dxgiAdapter()		{ return _dxgiAdapter; }

		DX11_ID3DDevice*		d3dDevice()			{ return _d3dDevice; }
		DX11_ID3DDeviceContext* d3dDeviceContext()	{ return _d3dDeviceContext; }
		DX11_ID3DDebug*			d3dDebug()			{ return _d3dDebug; }
	protected:
		ComPtr<DX11_IDXGIFactory>		_dxgiFactory;
		ComPtr<DX11_IDXGIDevice>		_dxgiDevice;
		ComPtr<DX11_IDXGIAdapter>		_dxgiAdapter;

		ComPtr<DX11_ID3DDevice>			_d3dDevice;
		ComPtr<DX11_ID3DDeviceContext>	_d3dDeviceContext;
		ComPtr<DX11_ID3DDebug>			_d3dDebug;

		virtual SPtr<RenderContext>		onCreateContext(RenderContextCreateDesc& desc) override;
		virtual SPtr<RenderGpuBuffer>	onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) override;
		virtual SPtr<Texture2D>			onCreateTexture2D(Texture2D_CreateDesc& desc) override;

		virtual SPtr<Material>			onCreateMaterial() override;
		virtual SPtr<Shader>			onCreateShader(StrView filename) override;
	};
}
#endif