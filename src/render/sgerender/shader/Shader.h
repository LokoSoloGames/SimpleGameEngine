#pragma once

#include <sgerender.h>
#include "ShaderLayout.h"
#include "ShaderInfo.h"

namespace SimpleGameEngine {
	class Shader;

	struct ShaderStage : public NonCopyable {
		const ShaderLayout* info() const { return &_info; }
	protected:
		ShaderLayout _info;
	};

	struct ShaderVertexStage : public ShaderStage {
		static constexpr ShaderStageMask stageMask() { return ShaderStageMask::Vertex; }
	};
	struct ShaderPixelStage : public ShaderStage {
		static constexpr ShaderStageMask stageMask() { return ShaderStageMask::Pixel; }
	};

	struct ShaderPass : public NonCopyable {
		ShaderPass(Shader* shader, ShaderInfo::Pass& info);

		virtual ~ShaderPass() = default;

		ShaderVertexStage* vertexStage() { return _vertexStage; }
		ShaderPixelStage* pixelStage() { return _pixelStage; }

	protected:
		Shader* _shader = nullptr;
		ShaderInfo::Pass* _info = nullptr;
		ShaderVertexStage* _vertexStage = nullptr;
		ShaderPixelStage* _pixelStage = nullptr;
	};

	class Shader : public RefCountBase {
	public:
		static Shader find(StrView& shaderName);

		Shader(StrView filename);
		virtual ~Shader();

		const String& filename() const { return _filename; }

		const ShaderInfo* info() const { return &_info; }

		Span<UPtr<ShaderPass>>	passes() { return _passes; }
	private:
		String	_filename;
		ShaderInfo _info;
		Vector_<UPtr<ShaderPass>, 1> _passes;
	};
}