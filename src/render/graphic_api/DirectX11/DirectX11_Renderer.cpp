#include "DirectX11_Renderer.h"
#include "DirectX11_RenderContext.h"
#include "DirectX11_RenderGpuBuffer.h"

#if SGE_RENDER_HAS_DX11
namespace SimpleGameEngine {
	DirectX11_Renderer::DirectX11_Renderer(CreateDesc& desc){
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

	RenderContext* DirectX11_Renderer::onCreateContext(RenderContextCreateDesc& desc) {
		DirectX11RenderContextCreateDesc dx11Desc;
		dx11Desc.window = static_cast<NativeUIWindowWin32*>(desc.window);
		return new DirectX11_RenderContext(dx11Desc);
	}

	RenderGpuBuffer* DirectX11_Renderer::onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) {
		return new DirectX11_RenderGpuBuffer(desc);
	}

	void DirectX11_Renderer::onReleaseCOM(void *pCOM) {
		if(pCOM == nullptr) return;
		((IUnknown*) pCOM)->Release();
	}

	void DirectX11_Renderer::onCompileVertexShader(wchar_t* fileName, const VertexLayout* layout, void*& pShader, void*& pVertexLayout) {
		HRESULT hr;

		ComPtr <DX11_ID3DBlob> bytecode;
		ComPtr <DX11_ID3DBlob> errorMsg;
		hr = D3DCompileFromFile(fileName, 0, 0, "VShader", "vs_4_0", 0, 0, bytecode.ptrForInit(), errorMsg.ptrForInit());
		Util::throwIfError(hr);
		DX11_ID3DVertexShader* pD3DShader;
		hr = _d3dDevice->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, &pD3DShader);
		Util::throwIfError(hr);
		pShader = pD3DShader;

		D3D11_INPUT_ELEMENT_DESC* inputDesc = new D3D11_INPUT_ELEMENT_DESC[layout->elements.size()];
		DX11_ID3DInputLayout* pD3DLayout;

		int index = 0; int size = 0;
		for (auto it = begin (layout->elements); it != end (layout->elements); ++it) {
			D3D11_INPUT_ELEMENT_DESC desc;
    		desc.SemanticName = VertexSemanticToStringC(it->semantic);
			desc.SemanticIndex = 0;
			switch (it->type) {
				case RenderDataType::FLOAT32x3: {
					desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				} break;
				case RenderDataType::FLOAT32x4: {
					desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				} break;
				case RenderDataType::UNORM8x4: {
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				} break;
				case RenderDataType::UINT32x4: {
					desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				} break;
			}
			desc.InputSlot = 0;
			desc.AlignedByteOffset = size;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			inputDesc[index] = desc;
			size += RenderDataTypeSize(it->type);
			index++;
			SGE_LOG("Create Input Layout: {0}", desc.Format);
		}
		hr = _d3dDevice->CreateInputLayout(inputDesc, index,bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &pD3DLayout);
		pVertexLayout = pD3DLayout;
		delete[] inputDesc;
		Util::throwIfError(hr);
		SGE_LOG("Finish Vertex Shader");
	}

	void DirectX11_Renderer::onCompilePixelShader(wchar_t* fileName, void*& pShader) {
		SGE_LOG("Compile Pixel Shader");
		HRESULT hr;

		ComPtr <DX11_ID3DBlob> bytecode;
		ComPtr <DX11_ID3DBlob> errorMsg;
		hr = D3DCompileFromFile(fileName, 0, 0, "PShader", "ps_4_0", 0, 0, bytecode.ptrForInit(), errorMsg.ptrForInit());
		Util::throwIfError(hr);

		DX11_ID3DPixelShader* pD3DShader;
		hr = _d3dDevice->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, &pD3DShader);
		pShader = pD3DShader;
		Util::throwIfError(hr);
		SGE_LOG("Finished Pixel Shader");
	}

	void DirectX11_Renderer::onReleaseShader(void *pShader) {
		SGE_LOG("Release Shader");
		onReleaseCOM(pShader);
	}

	void DirectX11_Renderer::onReleaseVertexLayout(void *pVertexLayout) {
		SGE_LOG("Release Vertex Layout");
		onReleaseCOM(pVertexLayout);
	}
}
#endif