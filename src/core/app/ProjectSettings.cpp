#include "ProjectSettings.h"
#include <sgecore/file/Directory.h>

namespace SimpleGameEngine {

	ProjectSettings* ProjectSettings::instance() {
		static ProjectSettings s;
		return &s;
	}

	void ProjectSettings::setProjectRoot(StrView path) {
		Directory::setCurrent(path);

		_projectRoot = path;

		//auto dir = Directory::getCurrent();
		//SGE_LOG("projectRoot = {}", dir);
	}

	StrView ProjectSettings::importedPath() const {
		return "LocalTemp/Imported";
	}

}