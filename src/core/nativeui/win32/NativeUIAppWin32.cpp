#include "NativeUIAppWin32.h"
#include <sgecore/error/Error.h>
#include <sgecore/string/UtfUtil.h>

#if SGE_OS_WINDOWS

namespace SimpleGameEngine {

	void NativeUIAppWin32::onCreate(CreateDesc &desc) {
		Base::onCreate(desc);
	}

	void NativeUIAppWin32::onRun() {
		Base::onRun();

		while (TRUE) {
			if (PeekMessage(&_win32_msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&_win32_msg);
				DispatchMessage(&_win32_msg);

				if (_win32_msg.message == WM_QUIT)
					break;
			} else {
				// Run game code here
				onUpdate();
			}
		}

		willQuit();
	}

	void NativeUIAppWin32::onQuit() {
		Base::onQuit();

		::PostQuitMessage(_exitCode);
	}

	String NativeUIAppWin32::onGetExecutableFilename() {
		wchar_t tmp[MAX_PATH + 1];
		if (!::GetModuleFileName(nullptr, tmp, MAX_PATH))
			throw SGE_ERROR("");

		String o = UtfUtil::toString(tmp);
		return o;
	}

	String NativeUIAppWin32::onGetCurrentDir() {
		wchar_t tmp[MAX_PATH+1];
		if (!::GetCurrentDirectory(MAX_PATH, tmp))
			throw SGE_ERROR("getCurrentDir");
		String o = UtfUtil::toString(tmp);
		return o;
	}

	void NativeUIAppWin32::onSetCurrentDir(StrView dir) {
		TempStringW tmp = UtfUtil::toStringW(dir);
		::SetCurrentDirectory(tmp.c_str());
	}
}

#endif
