#pragma once

#include <sgerender.h>
#include "ShaderLayout.h"

namespace SimpleGameEngine {
	class Shader : public NonCopyable {
	public:
		static Shader find(StrView& shaderName);
	private:
		ShaderLayout _layout;
	};
}