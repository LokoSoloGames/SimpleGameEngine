#pragma once

#include <sgecore/string/Lexer.h>
#include <sgerender/shader/ShaderInfo.h>

namespace SimpleGameEngine {

	class ShaderParser : public Lexer {
	public:
		void readFile(ShaderInfo& outInfo, StrView filename);
		void readMem(ShaderInfo& outInfo, ByteSpan data, StrView filename);

	private:
		void _readShader();
		void _readProperties();
		void _readProperty();
		void _readPass();

		void _readBlendFunc(RenderState::BlendFunc& v);

		MemMapFile _memMapFile;
		ShaderInfo* _outInfo = nullptr;
	};
}