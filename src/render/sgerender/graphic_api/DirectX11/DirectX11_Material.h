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

			Span<ConstBuffer> constBuffers() { return _constBuffers; }
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

			Span<ConstBuffer> constBuffers() { return _constBuffers; }
			DirectX11_Shader::MyPixelStage* shaderStage() { return static_cast<DirectX11_Shader::MyPixelStage*>(_shaderStage); }
		};

		struct MyPass : public Pass {
			MyPass(Material* material, ShaderPass* shaderPass);

			virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) override;

			MyVertexStage _myVertexStage;
			MyPixelStage  _myPixelStage;

		private:
			void onBindBlendState(DirectX11_RenderContext* ctx);

			ShaderInfo::Pass* _info;
			ComPtr<DX11_ID3DRasterizerState>	rasterizerState;
			ComPtr<DX11_ID3DDepthStencilState>	depthStencilState;
			ComPtr<DX11_ID3DBlendState>			blendState;
		};

		virtual Pass* onCreatePass(Material* material, ShaderPass* shaderPass) override {
			return new MyPass(material, shaderPass);
		}
	};
}