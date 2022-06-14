#pragma once

#include <sgecore/error/Error.h>

namespace SimpleGameEngine {

	struct ProjectSettings : public NonCopyable {

		static ProjectSettings* instance();

		void setProjectRoot(StrView path);

		const String&	projectRoot() const { return _projectRoot; }
		StrView	importedPath() const;

	private:
		String _projectRoot;
	};

}