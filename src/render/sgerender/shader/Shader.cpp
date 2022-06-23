#include "Shader.h"
#include <sgerender/Renderer.h>

namespace SimpleGameEngine {
	Shader::Shader(StrView filename) : _filename(filename) {
		auto* proj = ProjectSettings::instance();
		auto infoFilename = Fmt("{}/{}/info.json", proj->importedPath(), filename);
		JsonUtil::readFile(infoFilename, _info);
	}

	Shader::~Shader() {
		auto* renderer = Renderer::current();
		renderer->onShaderDestroy(this);
	}

	ShaderPass::ShaderPass(Shader* shader, ShaderInfo::Pass& info)
		: _shader(shader) , _info(&info) { }
}