#include "NativeUIAppBase.h"

namespace SimpleGameEngine {

	int NativeUIAppBase::run(CreateDesc& desc) {
		onCreate(desc);
		onRun();
		return _exitCode;
	}

	void NativeUIAppBase::quit(int exitCode) {
		_exitCode = exitCode;
		onQuit();
	}

}