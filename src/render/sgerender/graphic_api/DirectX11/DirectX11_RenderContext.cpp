#include "DirectX11_RenderContext.h"
#include "DirectX11_RenderGpuBuffer.h"
#include "DirectX11_Renderer.h"
#include "DirectX11_Material.h"

namespace SimpleGameEngine {
	DirectX11_RenderContext::DirectX11_RenderContext(DirectX11RenderContextCreateDesc& desc) : Base(desc) {
		_window = desc.window;
		_renderer = DirectX11_Renderer::instance();

		auto* dev = _renderer->d3dDevice();
		auto* dxgiFactory = _renderer->dxgiFactory();
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1;                                    // one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
		scd.OutputWindow = desc.window->_hwnd;                  // the window to be used
		scd.SampleDesc.Count = 4;                               // how many multisamples
		scd.Windowed = TRUE;                                    // windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

		auto hr = dxgiFactory->CreateSwapChain(dev, &scd, _swapChain.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_RenderContext::_createRenderTarget() {
		auto* renderer = DirectX11_Renderer::instance();
		auto* dev = renderer->d3dDevice();
		HRESULT hr;

		// get the address of the back buffer
		ComPtr <ID3D11Texture2D> backBuffer;
		hr = _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.ptrForInit()));
		Util::throwIfError(hr);

		// use the back buffer address to create the render target
		hr = dev->CreateRenderTargetView(backBuffer, NULL, _renderTargetView.ptrForInit());
		Util::throwIfError(hr);
	
		D3D11_TEXTURE2D_DESC backBufferDesc;
		backBuffer->GetDesc(&backBufferDesc);

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = backBufferDesc.Width;
		descDepth.Height = backBufferDesc.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = dev->CreateTexture2D(&descDepth, nullptr, _depthStencil.ptrForInit());
		Util::throwIfError(hr);

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = dev->CreateDepthStencilView(_depthStencil.ptr(), &descDSV, _depthStencilView.ptrForInit());
		Util::throwIfError(hr);
	}

	void DirectX11_RenderContext::onBeginRender() {
		auto* ctx = _renderer->d3dDeviceContext();
		if (!_renderTargetView) {
			_createRenderTarget();
		}

		DX11_ID3DRenderTargetView* rt[] = {_renderTargetView};
		ctx->OMSetRenderTargets(1, rt, nullptr);

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = _window->clientRect().w;
		viewport.Height = _window->clientRect().h;

		ctx->RSSetViewports(1, &viewport);
	}

	void DirectX11_RenderContext::onEndRender() {

	}

	void DirectX11_RenderContext::onCommit(RenderCommandBuffer& cmdBuf) {
		_dispatch(this, cmdBuf);
	}

	void DirectX11_RenderContext::onCmd_ClearFrameBuffers(RenderCommand_ClearFrameBuffers& cmd) {
		auto* ctx = _renderer->d3dDeviceContext();
		if (_renderTargetView && cmd.color.has_value()) {
			ctx->ClearRenderTargetView(_renderTargetView, cmd.color->data);
		}
		if (_depthStencilView && cmd.depth.has_value()) {
			ctx->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, *cmd.depth, 0);
		}
	}

	void DirectX11_RenderContext::onCmd_SwapBuffers(RenderCommand_SwapBuffers& cmd) {
		auto hr = _swapChain->Present(_renderer->vsync() ? 1 : 0, 0);
		Util::throwIfError(hr);
	}

	void DirectX11_RenderContext::onCmd_DrawCall(RenderCommand_DrawCall& cmd) {
		if (!cmd.vertexLayout) { SGE_ASSERT(false); return; }

		auto* vertexBuffer = static_cast<DirectX11_RenderGpuBuffer*>(cmd.vertexBuffer.ptr());
		if (!vertexBuffer) { SGE_ASSERT(false); return; }

		if (cmd.vertexCount <= 0) { SGE_ASSERT(false); return; }
		if (cmd.primitive == RenderPrimitiveType::None) { SGE_ASSERT(false); return; }

		DirectX11_RenderGpuBuffer* indexBuffer = nullptr;
		if (cmd.indexCount > 0) {
			indexBuffer = static_cast<DirectX11_RenderGpuBuffer*>(cmd.indexBuffer.ptr());
			if (!indexBuffer) { SGE_ASSERT(false); return; }
		}

		auto* pass = cmd.getMaterialPass();
		if(!pass) { SGE_ASSERT(false); return; }

		auto* ctx = _renderer->d3dDeviceContext();
		pass->bind(this, cmd.vertexLayout);
		auto primitive = Util::getPrimitiveTopology(cmd.primitive);
		ctx->IASetPrimitiveTopology(primitive);

		// select which vertex buffer to display
		UINT offset = 0;
		UINT stride = static_cast<UINT>(cmd.vertexLayout->stride);
		UINT vertexCount = static_cast<UINT>(cmd.vertexCount);
		UINT indexCount = static_cast<UINT>(cmd.indexCount);

		DX11_ID3DBuffer* pVBuffer[] = { vertexBuffer->getBuffer() };
		ctx->IASetVertexBuffers(0, 1, pVBuffer, &stride, &offset);
		
		if (indexCount > 0) {
			auto indexType = Util::getFormat(cmd.indexType);
			ctx->IASetIndexBuffer(indexBuffer->getBuffer(), indexType, 0);
			ctx->DrawIndexed(indexCount, 0, 0);
		} else {
			ctx->Draw(vertexCount, 0);
		}
	}

	void DirectX11_RenderContext::onSetFrameBufferSize(Vec2f newSize) {
		_renderTargetView.reset(nullptr); // release buffer and render target view before resize
		_depthStencilView.reset(nullptr);

		auto hr = _swapChain->ResizeBuffers(0
			, static_cast<UINT>(Math::max(0.0f, newSize.x))
			, static_cast<UINT>(Math::max(0.0f, newSize.y))
			, DXGI_FORMAT_UNKNOWN
			, 0);
		Util::throwIfError(hr);
	}
}