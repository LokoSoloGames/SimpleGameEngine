#pragma once

#include <sgerender/shader/ShaderInfo.h>
#include <sgerender/shader/ShaderLayout.h>
#include <sgerender/graphic_api/DirectX11/DirectX11_Common.h>

namespace SimpleGameEngine {
	class ShaderCompiler_DX11 : public NonCopyable {
		using Util = DX11Util;
	public:
		void compile(const StrView& outFileName, const ShaderStage& shaderStage, const StrView& srcFileName, const StrView& entryFunc, ShaderLayout& layout);
	private:
		// To be abstracted for different graphic api
		void compileVS(const StrView& shader, const StrView& alias, const StrView& vsName);
		void compilePS(const StrView& shader, const StrView& alias, const StrView& psName);

		void createLayout(ComPtr<DX11_ID3DShaderReflection>& reflection, ShaderLayout& layout);

		StrView _dirPath;
	};
}