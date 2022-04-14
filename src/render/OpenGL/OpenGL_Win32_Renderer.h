#pragma once

#include "../base/RendererBase.h"

#if SGE_OS_WINDOWS
#include <gl/glew.h>
#include <gl/wglew.h>
#pragma comment(lib, "glew32.lib")

namespace SimpleGameEngine {
	class OpenGL_Win32_Renderer : public RendererBase { // TODO Refactor to OpenGL_Base_Renderer
	protected:
		virtual void onInit(NativeUIWindow &window, CreateDesc &desc) override;
		virtual void onRender() override;
		virtual void onCleanUp() override;
	private:
		HWND	m_hwnd = nullptr;
		HDC		m_dc = nullptr;
		HGLRC	m_rc = nullptr;
	};
}
#endif