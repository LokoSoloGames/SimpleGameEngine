#pragma once

#include <sgerender/shader/Shader.h>
#include <sgerender/graphic_api/DirectX11/DirectX11_Common.h>

namespace SimpleGameEngine {
	class DirectX11_Shader : public Shader {
		using Base = Shader;
	public:
		using Util = DX11Util;

		DirectX11_Shader(StrView filename);
	};
}