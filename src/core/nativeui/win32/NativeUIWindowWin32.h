#pragma once

#include "../base/NativeUIWindowBase.h"

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

private:
	static LRESULT WINAPI s_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	SGE_INLINE static This* s_getThis(HWND hwnd) {
		return reinterpret_cast<This*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	LRESULT _handleNativeEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool _handleNativeUIMouseEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	UIEventModifier _getWin32Modifier();
};

}

#endif