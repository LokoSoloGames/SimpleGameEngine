#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"

#if SGE_RENDER_HAS_DX11
namespace SimpleGameEngine {
	DirectX11Renderer::DirectX11Renderer(CreateDesc& desc){
		D3D_FEATURE_LEVEL featureLevel;
		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ComPtr<ID3D11Device>			d3dDevice;
		ComPtr<ID3D11DeviceContext>		d3dDeviceContext;

		HRESULT hr;
		hr = D3D11CreateDevice(	nullptr,
								   D3D_DRIVER_TYPE_HARDWARE,
								   nullptr,
								   createDeviceFlags,
								   nullptr,
								   0,
								   D3D11_SDK_VERSION,
								   d3dDevice.ptrForInit(),
								   &featureLevel,
								   d3dDeviceContext.ptrForInit());
		Util::throwIfError(hr);

		hr = d3dDevice->QueryInterface(IID_PPV_ARGS(_d3dDevice.ptrForInit()));
		Util::throwIfError(hr);

		hr = d3dDeviceContext->QueryInterface(IID_PPV_ARGS(_d3dDeviceContext.ptrForInit()));
		Util::throwIfError(hr);

		hr = _d3dDevice->QueryInterface(IID_PPV_ARGS(_dxgiDevice.ptrForInit()));
		Util::throwIfError(hr);

		{
			ComPtr<IDXGIAdapter> adapter;
			hr = _dxgiDevice->GetAdapter(adapter.ptrForInit());
			Util::throwIfError(hr);

			hr = adapter->QueryInterface(IID_PPV_ARGS(_dxgiAdapter.ptrForInit()));
			Util::throwIfError(hr);
		}

		hr = _dxgiAdapter->GetParent(IID_PPV_ARGS(_dxgiFactory.ptrForInit()));
		Util::throwIfError(hr);

		if (createDeviceFlags & D3D11_CREATE_DEVICE_DEBUG) {
			hr = _d3dDevice->QueryInterface(_d3dDebug.ptrForInit());
			Util::throwIfError(hr);
		}
	}

	RenderContext* DirectX11Renderer::onCreateContext(RenderContextCreateDesc& desc) {
		DirectX11RenderContextCreateDesc dx11Desc;
		dx11Desc.window = static_cast<NativeUIWindowWin32*>(desc.window);
		return new DirectX11RenderContext(dx11Desc);
	}
}
#endif