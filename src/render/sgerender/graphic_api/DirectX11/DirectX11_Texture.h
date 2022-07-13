#pragma once

#include <sgerender/textures/Texture.h>
#include "DirectX11_Common.h"

namespace SimpleGameEngine {

class DirectX11_Texture : public Texture2D {
	using Base = Texture2D;
	using Util = DX11Util;
public:
	DirectX11_Texture(CreateDesc& desc);

	DX11_ID3DShaderResourceView*	resourceView() { return _resourceView.ptr(); }
	DX11_ID3DSamplerState*			samplerState() { return _samplerState.ptr(); }

private:
	ComPtr<DX11_ID3DTexture2D>			_tex;
	ComPtr<DX11_ID3DShaderResourceView>	_resourceView;
	ComPtr<DX11_ID3DSamplerState>		_samplerState;
};

}