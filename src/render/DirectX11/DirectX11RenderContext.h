#pragma once

#if SGE_RENDER_HAS_DX11

#include <nativeui/NativeUI.h>
#include <RenderContext.h>
#include "DirectX11Renderer.h"
#include "Common.h"

namespace SimpleGameEngine {
	struct DirectX11RenderContextCreateDesc : public RenderContextCreateDesc {
		NativeUIWindowWin32*	window = nullptr;
	};

	class DirectX11RenderContext : public RenderContext {
		using Base = RenderContext;
		using Util = DX11Util;
	public:
		DirectX11RenderContext(DirectX11RenderContextCreateDesc& desc);

	protected:
		DirectX11Renderer*	_renderer = nullptr;

		ComPtr<DX11_IDXGISwapChain>			_swapChain;
		ComPtr<DX11_ID3DRenderTargetView>	_renderTargetView;

		ComPtr<DX11_ID3DBuffer>			_testVertexBuffer;
		ComPtr<DX11_ID3DVertexShader>	_testVertexShader;
		ComPtr<DX11_ID3DPixelShader>	_testPixelShader;
		ComPtr<DX11_ID3DInputLayout>	_testInputLayout;

		void _createRenderTarget();

		virtual void onBeginRender() override;
		virtual void onEndRender() override;

		virtual void onRender() override;

		virtual void onClearBuffers() override;
		virtual void onSwapBuffers() override;
	private:
		NativeUIWindowWin32* _window;
	};

}
#endif