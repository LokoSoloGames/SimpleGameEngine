#pragma once

#include <sgerender/shader/ShaderInfo.h>
#include <sgerender/graphic_api/DirectX11/DirectX11_Common.h>

namespace SimpleGameEngine {
	class ShaderCompiler : public NonCopyable {
	public:
		void compile(const StrView& fileName);
	private:
		// To be abstracted for different graphic api
		void compileVS(const StrView& shader, const StrView& alias, const StrView& vsName);
		void compilePS(const StrView& shader, const StrView& alias, const StrView& psName);

		ShaderInfo createInfo(ComPtr<DX11_ID3DShaderReflection>& reflection);

		StrView _dirPath;
		Map<String, ShaderInfo> compiledShader;
	};
}