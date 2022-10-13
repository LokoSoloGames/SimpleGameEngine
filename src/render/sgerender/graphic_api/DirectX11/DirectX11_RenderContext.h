#pragma once

#include <nativeui/NativeUI.h>
#include <sgerender.h>
#include "DirectX11_Renderer.h"

#if SGE_RENDER_HAS_DX11

namespace SimpleGameEngine {
	struct DirectX11RenderContextCreateDesc : public RenderContextCreateDesc {
		NativeUIWindowWin32*	window = nullptr;
	};

	class DirectX11_RenderContext : public RenderContext {
		using Base = RenderContext;
		using Util = DX11Util;
	public:
		DirectX11_RenderContext(DirectX11RenderContextCreateDesc& desc);

		void onCmd_ClearFrameBuffers(RenderCommand_ClearFrameBuffers&	cmd);
		void onCmd_SwapBuffers		(RenderCommand_SwapBuffers&			cmd);
		void onCmd_DrawCall			(RenderCommand_DrawCall&			cmd);
		void onCmd_SetScissorRect	(RenderCommand_SetScissorRect&		cmd);

		DirectX11_Renderer* renderer() { return _renderer; }

	protected:
		DirectX11_Renderer*	_renderer = nullptr;

		ComPtr<DX11_IDXGISwapChain>			_swapChain;
		ComPtr<DX11_ID3DRenderTargetView>	_renderTargetView;
		ComPtr<DX11_ID3DTexture2D>			_depthStencil;
		ComPtr<DX11_ID3DDepthStencilView>	_depthStencilView;

		void _createRenderTarget();

		virtual void onSetFrameBufferSize(Vec2f newSize);
		virtual void onBeginRender() override;
		virtual void onEndRender() override;

		virtual void onCommit(RenderCommandBuffer& cmdBuf) override;
	private:
		NativeUIWindowWin32* _window;
	};
}
#endif