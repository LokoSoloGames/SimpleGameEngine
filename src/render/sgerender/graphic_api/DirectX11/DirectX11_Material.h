#pragma once

#include <sgerender/material/Material.h>
#include "DirectX11_Shader.h"
#include "DirectX11_RenderContext.h"

namespace SimpleGameEngine {
	class DirectX11_Material : public Material {
	public:

	private:
		DirectX11_Shader* shader() { return static_cast<DirectX11_Shader*>(_shader.ptr()); }

		template<class STAGE>
		static void _bindStageHelper(DirectX11_RenderContext* ctx, STAGE* stage);

		struct MyVertexStage : public VertexStage {
			using Base = VertexStage;

			MyVertexStage(MaterialPass* pass, ShaderVertexStage* shaderStage) : Base(pass, shaderStage) {}

			void bind(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout);
			void bindInputLayout(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout);

			void _dxSetConstBuffer(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DBuffer* d3dBuf) {
				dc->VSSetConstantBuffers(bindPoint, 1, &d3dBuf);
			}

			void _dxSetShaderResource(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DShaderResourceView* rv) {
				dc->VSSetShaderResources(bindPoint, 1, &rv);
			}

			void _dxSetSampler(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DSamplerState* ss) {
				dc->VSSetSamplers(bindPoint, 1, &ss);
			}

			Span<ConstBuffer>	constBuffers()	{ return _constBuffers; }
			Span<TexParam>		texParams()		{ return _texParams; }
			DirectX11_Shader::MyVertexStage* shaderStage() { return static_cast<DirectX11_Shader::MyVertexStage*>(_shaderStage); }

			VectorMap<const VertexLayout*, ComPtr<DX11_ID3DInputLayout>> _inputLayoutsMap;
		};

		struct MyPixelStage : public PixelStage {
			using Base = PixelStage;
			using MyShaderStage = DirectX11_Shader::MyPixelStage;

			MyPixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage) : Base(pass, shaderStage) {}
			void bind(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout);

			void _dxSetConstBuffer(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DBuffer* d3dBuf) {
				dc->PSSetConstantBuffers(bindPoint, 1, &d3dBuf);
			}

			void _dxSetShaderResource(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DShaderResourceView* rv) {
				dc->PSSetShaderResources(bindPoint, 1, &rv);
			}

			void _dxSetSampler(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DSamplerState* ss) {
				dc->PSSetSamplers(bindPoint, 1, &ss);
			}

			Span<ConstBuffer>	constBuffers()	{ return _constBuffers; }
			Span<TexParam>		texParams()		{ return _texParams; }
			DirectX11_Shader::MyPixelStage* shaderStage() { return static_cast<DirectX11_Shader::MyPixelStage*>(_shaderStage); }
		};

		struct MyPass : public Pass {
			MyPass(Material* material, ShaderPass* shaderPass);

			virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) override;

			MyVertexStage _myVertexStage;
			MyPixelStage  _myPixelStage;

		private:
			void onBindRasterizerState(DirectX11_RenderContext* ctx);
			void onBindDepthStencilState(DirectX11_RenderContext* ctx);
			void onBindBlendState(DirectX11_RenderContext* ctx);

			ComPtr<DX11_ID3DRasterizerState>	rasterizerState;
			ComPtr<DX11_ID3DDepthStencilState>	depthStencilState;
			ComPtr<DX11_ID3DBlendState>			blendState;
		};

		virtual UPtr<Pass> onCreatePass(ShaderPass* shaderPass) override {
			return UPtr<Pass>(new MyPass(this, shaderPass));
		}
	};
}