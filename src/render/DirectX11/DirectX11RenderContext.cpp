#include "DirectX11RenderContext.h"

namespace SimpleGameEngine {
	DirectX11RenderContext::DirectX11RenderContext(DirectX11RenderContextCreateDesc& desc) : Base(desc) {
		_window = desc.window;
		_renderer = DirectX11Renderer::current();

		auto* dev			= _renderer->d3dDevice();
		auto* dxgiFactory	= _renderer->dxgiFactory();
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1;									// one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
		scd.OutputWindow = desc.window->_hwnd;					// the window to be used
		scd.SampleDesc.Count = 4;								// how many multisamples
		scd.Windowed = TRUE;									// windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// allow full-screen switching

		auto hr = dxgiFactory->CreateSwapChain(dev, &scd, _swapChain.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11RenderContext::_createRenderTarget() {
		auto* renderer = DirectX11Renderer::current();
		auto* dev = renderer->d3dDevice();
		HRESULT hr;

		// get the address of the back buffer
		ComPtr<ID3D11Texture2D> backBuffer;
		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) backBuffer.ptrForInit());
		Util::throwIfError(hr);

		// use the back buffer address to create the render target
		dev->CreateRenderTargetView(backBuffer, NULL, _renderTargetView.ptrForInit());
	}

	void DirectX11RenderContext::onBeginRender() {
		auto* ctx = _renderer->d3dDeviceContext();
		if (!_renderTargetView) {
			_createRenderTarget();
		}

		DX11_ID3DRenderTargetView* rt[] = { _renderTargetView };
		ctx->OMSetRenderTargets(1, rt, nullptr);
	}

	void DirectX11RenderContext::onEndRender() {

	}

	void DirectX11RenderContext::onRender() {
		auto* dev = _renderer->d3dDevice();
		auto* ctx = _renderer->d3dDeviceContext();

		HRESULT hr;

// testing
		struct Vertex {
			float x,y,z;
			u8 color[4];
		};

		static Vertex vertexData[] =
				{
						{ 0.0f,  0.5f, 0.0f, { 255, 0, 0, 255 }},
						{ 0.5f, -0.5f, 0.0f, { 0, 255, 0, 255 }},
						{-0.5f, -0.5f, 0.0f, { 0, 0, 255, 255 }}
				};

		UINT vertexCount = sizeof(vertexData) / sizeof(vertexData[0]);

		if (!_testVertexBuffer) {

			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
			bd.ByteWidth = sizeof(Vertex) * vertexCount;;	// size is the VERTEX struct * 3
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer
			hr = dev->CreateBuffer(&bd, nullptr, _testVertexBuffer.ptrForInit());  // create the buffer
			Util::throwIfError(hr);

			// copy the vertices into the buffer
			D3D11_MAPPED_SUBRESOURCE ms = {};
			hr = ctx->Map(_testVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
			Util::throwIfError(hr);

			memcpy(ms.pData, vertexData, bd.ByteWidth); // copy the data
			ctx->Unmap(_testVertexBuffer, NULL);	 // unmap the buffer
		}

		const wchar_t* shaderFile = L"shaders.shader";

		if (!_testVertexShader) {
			ComPtr<ID3DBlob>	bytecode;
			ComPtr<ID3DBlob>	errorMsg;
			hr = D3DCompileFromFile(shaderFile, 0, 0, "VShader", "vs_4_0", 0, 0, bytecode.ptrForInit(), errorMsg.ptrForInit());
			Util::throwIfError(hr);

			hr = dev->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, _testVertexShader.ptrForInit());
			Util::throwIfError(hr);

			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			hr = dev->CreateInputLayout(inputDesc
					, sizeof(inputDesc) / sizeof(inputDesc[0])
					, bytecode->GetBufferPointer()
					, bytecode->GetBufferSize()
					, _testInputLayout.ptrForInit());
			Util::throwIfError(hr);
		}

		if (!_testPixelShader) {
			ComPtr<ID3DBlob>	bytecode;
			ComPtr<ID3DBlob>	errorMsg;
			hr = D3DCompileFromFile(shaderFile, 0, 0, "PShader", "ps_4_0", 0, 0, bytecode.ptrForInit(), errorMsg.ptrForInit());
			Util::throwIfError(hr);

			dev->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, _testPixelShader.ptrForInit());
			Util::throwIfError(hr);
		}

// draw
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX	 = 0;
		viewport.TopLeftY	 = 0;
		viewport.Width		 = _window->viewRect.w;
		viewport.Height		 = _window->viewRect.h;

		ctx->RSSetViewports(1, &viewport);

		// select which primitive type we are using
		ctx->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// set the input layout we are using
		ctx->IASetInputLayout(_testInputLayout);

		// select which vertex buffer to display
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* pVBuffer[] = { _testVertexBuffer };
		ctx->IASetVertexBuffers(0, 1, pVBuffer, &stride, &offset);

		ctx->VSSetShader(_testVertexShader, 0, 0);
		ctx->PSSetShader(_testPixelShader,  0, 0);

		ctx->Draw(vertexCount, 0);
	}

	void DirectX11RenderContext::onClearBuffers() {
		auto* ctx = _renderer->d3dDeviceContext();
		if (_renderTargetView) {
			// clear the render target
			float color[4] = {0, 0.2, 0.4f, 1};
			ctx->ClearRenderTargetView(_renderTargetView,  color);
		}
	}

	void DirectX11RenderContext::onSwapBuffers() {
		auto hr = _swapChain->Present(_renderer->vsync() ? 1 : 0, 0);
		Util::throwIfError(hr);
	}
}