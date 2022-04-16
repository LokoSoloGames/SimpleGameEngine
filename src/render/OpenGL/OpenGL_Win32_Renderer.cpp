#include "OpenGL_Win32_Renderer.h"

#if SGE_RENDER_HAS_OPENGL
namespace SimpleGameEngine {
//	void OpenGL_Win32_Renderer::onInit(NativeUIWindow &window, CreateDesc &desc) {
//		m_hwnd = window._hwnd;
//		m_dc = GetDC(m_hwnd);
//		if (!m_dc)
//			throw SGE_ERROR("GetDC Failed");
//
//		PIXELFORMATDESCRIPTOR pfd;
//		memset(&pfd, 0, sizeof(pfd));
//		pfd.nSize = sizeof(pfd);
//		pfd.nVersion = 1;
//		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
//		pfd.iPixelType = PFD_TYPE_RGBA;
//		pfd.cColorBits = 32;
//		pfd.cDepthBits = 32;
//		pfd.iLayerType = PFD_MAIN_PLANE;
//
//		int pf = ChoosePixelFormat(m_dc, &pfd);
//		if (pf == 0)
//			throw SGE_ERROR("ChoosePixelFormat");
//
//		if (!SetPixelFormat(m_dc, pf, &pfd))
//			throw SGE_ERROR("SetPixelFormat");
//
//		HGLRC tempContext = wglCreateContext(m_dc);
//		if (!tempContext)
//			throw SGE_ERROR("wglCreateContext");
//
//		wglMakeCurrent(m_dc, tempContext);
//		GLenum err = glewInit();
//		if (glewInit() != GLEW_OK)
//			throw SGE_ERROR("GLEW is not initialized!");
//
//		int attribs[] = {
//			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
//			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
//			WGL_CONTEXT_FLAGS_ARB, 0,
//			0
//		};
//
//		if(wglewIsSupported("WGL_ARB_create_context") == 1) {
//			m_rc = wglCreateContextAttribsARB(m_dc, 0, attribs);
//			wglMakeCurrent(NULL,NULL);
//			wglDeleteContext(tempContext);
//			wglMakeCurrent(m_dc, m_rc);
//		} else {
//			//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
//			m_rc = tempContext;
//		}
//	}
}
#endif