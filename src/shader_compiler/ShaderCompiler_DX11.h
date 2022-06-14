#pragma once

#include <sgerender/shader/ShaderInfo.h>
#include <sgerender/graphic_api/DirectX11/DirectX11_Common.h>

namespace SimpleGameEngine {
	class ShaderCompiler_DX11 : public NonCopyable {
		using Util = DX11Util;
		using DataType = RenderDataType;
	public:
		void compile(StrView outFilename, ShaderStageMask shaderStage, StrView srcFilename, StrView entryFunc);

	private:
		void _reflect(StrView outFilename, ByteSpan bytecode, ShaderStageMask stage, StrView profile);
		void _reflect_inputs(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc);
		void _reflect_constBuffers(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc);
		void _reflect_textures(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc);
		void _reflect_samplers(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc);
	};
}