#include "NativeUIAppWin32.h"

#if SGE_OS_WINDOWS

namespace SimpleGameEngine {

void NativeUIAppWin32::onCreate(CreateDesc& desc) {
	Base::onCreate(desc);
}

void NativeUIAppWin32::onRun() {
	Base::onRun();

	while (GetMessage(&_win32_msg, NULL, 0, 0)) {
		TranslateMessage(&_win32_msg);
		DispatchMessage(&_win32_msg);
	}

	willQuit();
}

void NativeUIAppWin32::onQuit() {
	Base::onQuit();

	::PostQuitMessage(_exitCode);
}

}

#endif
