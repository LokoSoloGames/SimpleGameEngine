#pragma once

#include "../base/NativeUIWindowBase.h"
#include <Windows.h>

#if SGE_OS_WINDOWS

namespace SimpleGameEngine {

class NativeUIWindowWin32 : public NativeUIWindowBase {
	using This = NativeUIWindowWin32;
	using Base = NativeUIWindowBase;
public:

	virtual void onCreate(CreateDesc& desc) override;
	virtual void onSetWindowTitle(StrView title) override;
	virtual void onDrawNeeded() override;

	HWND _hwnd;
	Rect2f viewRect;

private:
	static LRESULT WINAPI s_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	SGE_INLINE static This* s_getThis(HWND hwnd) {
		return reinterpret_cast<This*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	LRESULT _handleWin32Event(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

}

#endif