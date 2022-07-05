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

		onBindBlendState(ctx);
		_myVertexStage.bind(ctx, vertexLayout);
		_myPixelStage.bind(ctx, vertexLayout);
	}

	DirectX11_Material::MyPass::MyPass(Material* material, ShaderPass* shaderPass)
		: Pass(material, shaderPass)
		, _myVertexStage(this, shaderPass->vertexStage())
		, _myPixelStage(this, shaderPass->pixelStage())
	{
		_vertexStage = &_myVertexStage;
		_pixelStage = &_myPixelStage;
		_info = shaderPass->info();
	}

	void DirectX11_Material::MyPass::onBindBlendState(DirectX11_RenderContext* ctx) {
		auto* dev = ctx->renderer()->d3dDevice();
		auto* dc  = ctx->renderer()->d3dDeviceContext();

		if (!blendState) {
			D3D11_BLEND_DESC blendStateDesc = {};
			blendStateDesc.AlphaToCoverageEnable = false;
			blendStateDesc.IndependentBlendEnable = false;
			auto& rtDesc = blendStateDesc.RenderTarget[0];

			rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			if (_info->blendRGB.enabled && _info->blendAlpha.enabled) {
				rtDesc.BlendEnable = true;
				rtDesc.BlendOp			= DX11Util::getBlendOp(_info->blendRGB.op);
				rtDesc.BlendOpAlpha		= DX11Util::getBlendOp(_info->blendAlpha.op);
				rtDesc.SrcBlend			= DX11Util::getBlendType(_info->blendRGB.src);
				rtDesc.DestBlend		= DX11Util::getBlendType(_info->blendRGB.dst);
				rtDesc.SrcBlendAlpha	= DX11Util::getBlendType(_info->blendAlpha.src);
				rtDesc.DestBlendAlpha	= DX11Util::getBlendType(_info->blendAlpha.dst);
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