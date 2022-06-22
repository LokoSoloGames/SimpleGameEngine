#pragma once

#include <sgerender/shader/Shader.h>
#include <sgerender/graphic_api/DirectX11/DirectX11_Common.h>
#include "DirectX11_RenderContext.h"

namespace SimpleGameEngine {
	class DirectX11_Shader : public Shader {
		using Base = Shader;
	public:
		using Util = DX11Util;

		DirectX11_Shader(StrView filename);

		struct MyPass;

		static void _loadStageFile(StrView passPath, ShaderStageMask stageMask, Vector<u8>& outBytecode, ShaderStageInfo& outInfo);

		struct MyVertexStage : public ShaderVertexStage {
			void load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev);
			void bind(DirectX11_RenderContext* ctx);
			ByteSpan bytecode() const { return _bytecode; }
		private:
			ComPtr<DX11_ID3DVertexShader>	_shader;
			Vector<u8> _bytecode;
		};

		struct MyPixelStage : public ShaderPixelStage {
			void load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev);
			void bind(DirectX11_RenderContext* ctx);
			ByteSpan bytecode() const { return _bytecode; }
		private:
			ComPtr<DX11_ID3DPixelShader>	_shader;
			Vector<u8> _bytecode;
		};

		struct MyPass : public ShaderPass {
			using Base = ShaderPass;

			MyPass(DirectX11_Shader* shader, StrView passPath, ShaderInfo::Pass& info);
		private:
			MyVertexStage		_myVertexStage;
			MyPixelStage		_myPixelStage;
		};
	};
}