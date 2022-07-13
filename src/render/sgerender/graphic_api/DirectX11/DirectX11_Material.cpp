#include "DirectX11_Material.h"
#include "DirectX11_Renderer.h"
#include "DirectX11_RenderGpuBuffer.h"

namespace SimpleGameEngine {
	template<class STAGE>
	void DirectX11_Material::_bindStageHelper(DirectX11_RenderContext* ctx, STAGE* stage) {
		auto* shaderStage = stage->shaderStage();
		if (!shaderStage) return;
		shaderStage->bind(ctx);

		auto* dc = ctx->renderer()->d3dDeviceContext();

		for (auto& cb : stage->constBuffers()) {
			cb.uploadToGpu();

			auto* cbInfo = cb.info();
			UINT bindPoint = cbInfo->bindPoint;

			auto* gpuBuffer = static_cast<DirectX11_RenderGpuBuffer*>(cb.gpuBuffer.ptr());
			if (!gpuBuffer) throw SGE_ERROR("cosnt buffer is null");

			auto* d3dBuf = gpuBuffer->getBuffer();
			if (!d3dBuf) throw SGE_ERROR("d3dbuffer is null");

			stage->_dxSetConstBuffer(dc, bindPoint, d3dBuf);
		}

		for (auto& texParam : stage->texParams()) {
			auto* tex = texParam.getUpdatedTexture();

			int bindPoint = texParam.bindPoint();

			switch (texParam.dataType()) {
			case RenderDataType::Texture2D: {
				auto* tex2d = static_cast<Texture2D_DX11*>(tex);
				auto* rv = tex2d->resourceView();
				auto* ss = tex2d->samplerState();

				stage->_dxSetShaderResource(dc, bindPoint, rv);
				stage->_dxSetSampler(dc, bindPoint, ss);
			} break;

			default: throw SGE_ERROR("bind unsupported texture type");
			}
		}
	}

	void DirectX11_Material::MyPixelStage::bind(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout) {
		_bindStageHelper(ctx, this);
	}

	void DirectX11_Material::MyVertexStage::bind(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout) {
		_bindStageHelper(ctx, this);

		bindInputLayout(ctx, vertexLayout);
	}

	void DirectX11_Material::MyVertexStage::bindInputLayout(DirectX11_RenderContext* ctx, const VertexLayout* vertexLayout) {
		if (!vertexLayout) throw SGE_ERROR("vertexLayout is null");

		auto* dev = ctx->renderer()->d3dDevice();
		auto* dc = ctx->renderer()->d3dDeviceContext();


		DX11_ID3DInputLayout* dxLayout = nullptr;

		auto it = _inputLayoutsMap.find(vertexLayout);
		if (it != _inputLayoutsMap.end()) {
			dxLayout = it->second;
		}
		else {
			Vector_<D3D11_INPUT_ELEMENT_DESC, 32> inputDesc;

			auto* vsInfo = info();
			for (auto& input : vsInfo->inputs) {
				auto* e = vertexLayout->find(input.semantic);
				if (!e) {
					throw SGE_ERROR("vertex sematic {} not found", input.semantic);
				}

				auto& dst = inputDesc.emplace_back();
				auto semanticType = VertexSemanticUtil::getType(e->semantic);
				dst.SemanticName = DX11Util::getSemanticName(semanticType);
				dst.SemanticIndex = VertexSemanticUtil::getIndex(e->semantic);
				dst.Format = DX11Util::getFormat(e->dataType);
				dst.InputSlot = 0;
				dst.AlignedByteOffset = e->offset;
				dst.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				dst.InstanceDataStepRate = 0;
			}

			ComPtr<DX11_ID3DInputLayout>	outLayout;

			auto* ss = static_cast<DirectX11_Shader::MyVertexStage*>(_shaderStage);
			auto bytecode = ss->bytecode();

			auto hr = dev->CreateInputLayout(inputDesc.data()
				, static_cast<UINT>(inputDesc.size())
				, bytecode.data()
				, bytecode.size()
				, outLayout.ptrForInit());
			DX11Util::throwIfError(hr);

			dxLayout = outLayout;
			_inputLayoutsMap[vertexLayout] = std::move(outLayout);
		}

		dc->IASetInputLayout(dxLayout);
	}

	void DirectX11_Material::MyPass::onBind(RenderContext* ctx_, const VertexLayout* vertexLayout) {
		auto* ctx = static_cast<DirectX11_RenderContext*>(ctx_);

		_myVertexStage.bind(ctx, vertexLayout);
		_myPixelStage.bind(ctx, vertexLayout);

		onBindRasterizerState(ctx);
		onBindDepthStencilState(ctx);
		onBindBlendState(ctx);
	}

	DirectX11_Material::MyPass::MyPass(Material* material, ShaderPass* shaderPass)
		: Pass(material, shaderPass)
		, _myVertexStage(this, shaderPass->vertexStage())
		, _myPixelStage(this, shaderPass->pixelStage())
	{
		_vertexStage = &_myVertexStage;
		_pixelStage = &_myPixelStage;
	}

	void DirectX11_Material::MyPass::onBindRasterizerState(DirectX11_RenderContext* ctx) {
		auto* dc = ctx->renderer()->d3dDeviceContext();

		if (!rasterizerState) {
			auto& rs = info()->renderState;
			auto* dev = ctx->renderer()->d3dDevice();

			D3D11_RASTERIZER_DESC rasterDesc = {};
			rasterDesc.AntialiasedLineEnable = true;
			rasterDesc.CullMode = DX11Util::getCullMode(rs.cull);
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;

			rasterDesc.FillMode = rs.wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;
			auto hr = dev->CreateRasterizerState(&rasterDesc, rasterizerState.ptrForInit());
			DX11Util::throwIfError(hr);
		}
		dc->RSSetState(rasterizerState);
	}

	void DirectX11_Material::MyPass::onBindDepthStencilState(DirectX11_RenderContext * ctx) {
		auto* dc = ctx->renderer()->d3dDeviceContext();

		if (!depthStencilState) {
			auto* dev = ctx->renderer()->d3dDevice();
			auto& rs = info()->renderState;

			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

			bool depthTest = rs.depthTest.isEnable();
			if (depthTest) {
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthFunc = DX11Util::getDepthTestOp(rs.depthTest.op);
			}
			else {
				depthStencilDesc.DepthEnable = false;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			}
			depthStencilDesc.DepthWriteMask = rs.depthTest.writeMask ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			depthStencilDesc.StencilEnable = false;
			depthStencilDesc.StencilReadMask = 0xFF;
			depthStencilDesc.StencilWriteMask = 0xFF;
			auto hr = dev->CreateDepthStencilState(&depthStencilDesc, depthStencilState.ptrForInit());
			DX11Util::throwIfError(hr);
		}
		dc->OMSetDepthStencilState(depthStencilState, 1);
	}

	void DirectX11_Material::MyPass::onBindBlendState(DirectX11_RenderContext* ctx) {
		auto* dc  = ctx->renderer()->d3dDeviceContext();

		if (!blendState) {
			auto* dev = ctx->renderer()->d3dDevice();
			auto& rs = info()->renderState;

			D3D11_BLEND_DESC blendStateDesc = {};
			blendStateDesc.AlphaToCoverageEnable = false;
			blendStateDesc.IndependentBlendEnable = false;
			auto& rtDesc = blendStateDesc.RenderTarget[0];

			rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			bool blendEnable = rs.blend.isEnable();
			if (blendEnable) {
				rtDesc.BlendEnable = true;
				if (rs.blend.rgb.op == RenderState_BlendOp::Disable) {
					rtDesc.BlendOp		= D3D11_BLEND_OP_ADD;
					rtDesc.SrcBlend		= D3D11_BLEND_ONE;
					rtDesc.DestBlend	= D3D11_BLEND_ZERO;
				} else {
					rtDesc.BlendOp		= DX11Util::getBlendOp(rs.blend.rgb.op);
					rtDesc.SrcBlend		= DX11Util::getBlendFactor(rs.blend.rgb.srcFactor);
					rtDesc.DestBlend	= DX11Util::getBlendFactor(rs.blend.rgb.dstFactor);
				}

				if (rs.blend.alpha.op == RenderState_BlendOp::Disable) {
					rtDesc.BlendOpAlpha		= D3D11_BLEND_OP_ADD;
					rtDesc.SrcBlendAlpha	= D3D11_BLEND_ONE;
					rtDesc.DestBlendAlpha	= D3D11_BLEND_ZERO;
				} else {
					rtDesc.BlendOpAlpha		= DX11Util::getBlendOp(rs.blend.alpha.op);
					rtDesc.SrcBlendAlpha	= DX11Util::getBlendFactor(rs.blend.alpha.srcFactor);
					rtDesc.DestBlendAlpha	= DX11Util::getBlendFactor(rs.blend.alpha.dstFactor);
				}
			} else {
				rtDesc.BlendEnable = false;
			}
			auto hr = dev->CreateBlendState(&blendStateDesc, blendState.ptrForInit());
			DX11Util::throwIfError(hr);
		}
		Color4f blendColor(1, 1, 1, 1);
		dc->OMSetBlendState(blendState, blendColor.data, 0xffffffff);
	}
}