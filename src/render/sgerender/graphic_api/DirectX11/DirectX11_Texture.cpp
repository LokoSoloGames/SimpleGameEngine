#include "DirectX11_Texture.h"

namespace SimpleGameEngine {
	
DirectX11_Texture2D::DirectX11_Texture2D(CreateDesc& desc) : Base(desc) {
	auto* renderer = DirectX11_Renderer::instance();
	auto* dev = renderer->d3dDevice();

	D3D11_TEXTURE2D_DESC dxDesc = {};
	dxDesc.Width = desc.size.x;
	dxDesc.Height = desc.size.y;
	dxDesc.MipLevels = desc.mipmapCount;

	dxDesc.ArraySize = 1;
	dxDesc.Format = Util::getColorType(desc.colorType);

	dxDesc.SampleDesc.Count = 1;
	dxDesc.SampleDesc.Quality = 0;
	dxDesc.Usage = D3D11_USAGE_DEFAULT;
	dxDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // D3D11_BIND_RENDER_TARGET
	dxDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
	dxDesc.MiscFlags = 0; // D3D11_RESOURCE_MISC_GENERATE_MIPS


	bool uploadImage = false;
	D3D11_SUBRESOURCE_DATA initData = {};

	if (desc.imageToUpload.colorType() != ColorType::None) {
		auto& info = desc.imageToUpload.info();

		initData.pSysMem = desc.imageToUpload.dataPtr();
		initData.SysMemPitch = info.strideInBytes; // Bytes for 1 row
		initData.SysMemSlicePitch = 0; // Only for 3D Texture

		uploadImage = true;
	}

	auto hr = dev->CreateTexture2D(&dxDesc, uploadImage ? &initData : nullptr, _tex.ptrForInit());
	Util::throwIfError(hr);

	//---------
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = Util::getTextureFilter(desc.samplerState.filter);
	samplerDesc.AddressU = Util::getTextureWrap(desc.samplerState.wrapU);
	samplerDesc.AddressV = Util::getTextureWrap(desc.samplerState.wrapV);
	samplerDesc.AddressW = Util::getTextureWrap(desc.samplerState.wrapW);
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = desc.samplerState.minLOD;
	samplerDesc.MaxLOD = desc.samplerState.maxLOD;

	hr = dev->CreateSamplerState(&samplerDesc, _samplerState.ptrForInit());
	Util::throwIfError(hr);

	//---------
	D3D11_SHADER_RESOURCE_VIEW_DESC rvDesc = {};
	rvDesc.Format = dxDesc.Format;
	rvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rvDesc.Texture2D.MipLevels = dxDesc.MipLevels ? dxDesc.MipLevels : -1;
	rvDesc.Texture2D.MostDetailedMip = 0;
	hr = dev->CreateShaderResourceView(_tex, &rvDesc, _resourceView.ptrForInit());
	Util::throwIfError(hr);
}

}