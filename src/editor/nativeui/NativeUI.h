#pragma once

#include <sgebase.h>

#if SGE_OS_WINDOWS

#include "win32/NativeUIAppWin32.h"
#include "win32/NativeUIWindowWin32.h"

namespace SimpleGameEngine {
	using NativeUIWindow_Impl	= NativeUIWindowWin32;
	using NativeUIApp_Impl		= NativeUIAppWin32;
}

#else
#error "unsupported platform"

#endif


namespace SimpleGameEngine {

	class NativeUIWindow : public NativeUIWindow_Impl {
	};

	class NativeUIApp : public NativeUIApp_Impl {
	public:
		static NativeUIApp* current() { return _current; }
		NativeUIApp() { _current = this; }

	private:
		static NativeUIApp* _current;
	};

}