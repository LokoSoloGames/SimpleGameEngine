#pragma once

#include "ShaderInfo.h"
#include <graphic_api/DirectX11/DirectX11_Common.h>

namespace SimpleGameEngine {
	class ShaderCompiler : public NonCopyable {
	public:
		void compile(const StrView& fileName);
	private:
		// To be abstracted for different graphic api
		void compileVS(const StrView& shader, const StrView& alias, const StrView& vsName);
		void compilePS(const StrView& shader, const StrView& alias, const StrView& psName);

		Shaderinfo createInfo(ComPtr<DX11_ID3DShaderReflection>& reflection);

		StrView _dirPath;
		Map<String, Shaderinfo> compiledShader;
	};
}