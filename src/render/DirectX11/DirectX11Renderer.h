#pragma once

#include "../base/RendererBase.h"

#if SGE_OS_WINDOWS
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
namespace SimpleGameEngine {
	class DirectX11Renderer : public RendererBase {
	protected:
		virtual void onInit(NativeUIWindow &window, CreateDesc &desc) override;
		virtual void onRender() override;
		virtual void onCleanUp() override;
	private:
		struct Color {
			union {
				struct { float r, g, b, a;};
				float data[4];
			};
			Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
		};

		struct VERTEX {
			FLOAT X, Y, Z;	// position
			Color color;	// color
		};

		IDXGISwapChain *swapchain;				// the pointer to the swap chain interface
		ID3D11Device *dev;						// the pointer to our Direct3D device interface
		ID3D11DeviceContext *devcon;			// the pointer to our Direct3D device context
		ID3D11RenderTargetView *backbuffer;

		ID3D11VertexShader *pVS;	// the vertex shader
		ID3D11PixelShader *pPS;		// the pixel shader
		ID3D11Buffer *pVBuffer;		// the vertex buffer
		ID3D11InputLayout *pLayout;	// the input layout

		void initPipeline(CreateDesc &desc);
		void initGraphics(CreateDesc &desc);
	};
}
#endif