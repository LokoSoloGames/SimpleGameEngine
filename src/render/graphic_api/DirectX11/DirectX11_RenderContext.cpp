#include "DirectX11_RenderContext.h"
#include "DirectX11_RenderGpuBuffer.h"

namespace SimpleGameEngine {
	DirectX11_RenderContext::DirectX11_RenderContext(DirectX11RenderContextCreateDesc& desc) : Base(desc) {
		_window = desc.window;
		_renderer = DirectX11_Renderer::current();

		auto* dev = _renderer->d3dDevice();
		auto* dxgiFactory = _renderer->dxgiFactory();
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1;                                    // one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;        // use 32-bit color
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;        // how swap chain is to be used
		scd.OutputWindow = desc.window->_hwnd;                    // the window to be used
		scd.SampleDesc.Count = 4;                                // how many multisamples
		scd.Windowed = TRUE;                                    // windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // allow full-screen switching

		auto hr = dxgiFactory->CreateSwapChain(dev, &scd, _swapChain.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_RenderContext::_createRenderTarget() {
		auto* renderer = DirectX11_Renderer::current();
		auto* dev = renderer->d3dDevice();
		HRESULT hr;

		// get the address of the back buffer
		ComPtr <ID3D11Texture2D> backBuffer;
		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) backBuffer.ptrForInit());
		Util::throwIfError(hr);

		// use the back buffer address to create the render target
		dev->CreateRenderTargetView(backBuffer, NULL, _renderTargetView.ptrForInit());
	}

	void DirectX11_RenderContext::onBeginRender() {
		auto* ctx = _renderer->d3dDeviceContext();
		if (!_renderTargetView) {
			_createRenderTarget();
		}

		DX11_ID3DRenderTargetView* rt[] = {_renderTargetView};
		ctx->OMSetRenderTargets(1, rt, nullptr);
	}

	void DirectX11_RenderContext::onEndRender() {

	}

	void DirectX11_RenderContext::onRender(RenderCommand_Draw& cmd) {
		auto* ctx = _renderer->d3dDeviceContext();

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = _window->viewRect.w;
		viewport.Height = _window->viewRect.h;

		ctx->RSSetViewports(1, &viewport);

		// select which primitive type we are using
		switch (cmd.renderMesh->primitive()) {
			case RenderPrimitiveType::Triangles: {
				ctx->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			} break;
			case RenderPrimitiveType::Lines: {
				ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			} break;
			default:
				SGE_LOG("Unhandled RenderPrimitiveType");
				break;
		}
		// set the input layout we are using
		ctx->IASetInputLayout(static_cast<DX11_ID3DInputLayout*>(cmd.shaderPass->vertexLayout));
		// select which vertex buffer to display
		UINT offset = 0;
		UINT stride = static_cast<UINT>(cmd.renderMesh->layout()->stride);
		DirectX11_RenderGpuBuffer* vertexBuffer = static_cast<DirectX11_RenderGpuBuffer*>(cmd.renderMesh->vertexBuf());
		DX11_ID3DBuffer* pVBuffer[] = { vertexBuffer->getBuffer() };
		ctx->IASetVertexBuffers(0, 1, pVBuffer, &stride, &offset);
		//ctx->IASetIndexBuffer(static_cast<DirectX11_RenderGpuBuffer*>(cmd.renderMesh->indexBuf().getBuffer()), DXGI_FORMAT_R32_UINT, 0);
		ctx->VSSetShader(static_cast<DX11_ID3DVertexShader*>(cmd.shaderPass->vertexShader), 0, 0);
		ctx->PSSetShader(static_cast<DX11_ID3DPixelShader*>(cmd.shaderPass->pixelShader), 0, 0);

		ctx->Draw(static_cast<UINT>(cmd.renderMesh->vertexCount()), 0); // Not using Index Buffer now
	}

	void DirectX11_RenderContext::onClearBuffers() {
		auto* ctx = _renderer->d3dDeviceContext();
		if (_renderTargetView) {
			// clear the render target
			float color[4] = {0, 0.2f, 0.4f, 1};
			ctx->ClearRenderTargetView(_renderTargetView, color);
		}
	}

	void DirectX11_RenderContext::onSwapBuffers() {
		auto hr = _swapChain->Present(_renderer->vsync() ? 1 : 0, 0);
		Util::throwIfError(hr);
	}
}