#pragma once

#include "DirectX11_Common.h"

#if SGE_RENDER_HAS_DX11
namespace SimpleGameEngine {
	class DirectX11_Renderer : public Renderer {
		using Base = Renderer;
		using Util = DX11Util;
	public:
		static DirectX11_Renderer* current() { return static_cast<DirectX11_Renderer*>(_current); }

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

		virtual RenderContext*	onCreateContext(RenderContextCreateDesc& desc) override;
		virtual RenderGpuBuffer* onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) override;
		virtual void onCompileVertexShader(wchar_t* fileName, const VertexLayout* layout, void*& pShader, void*& pVertexLayout) override;
		virtual void onCompilePixelShader(wchar_t* fileName, void*& pShader) override;
		virtual void onReleaseShader(void* pShader) override;
		virtual void onReleaseVertexLayout(void* pVertexLayout) override;
	private:
		void onReleaseCOM(void* pCOM);
	};
}
#endif