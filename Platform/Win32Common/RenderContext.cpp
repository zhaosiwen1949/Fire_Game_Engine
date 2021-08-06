#include "RenderContext.h"
#include "Runtime/Debugger/Log.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
namespace Editor
{
	static const DWORD kColorChannelBits = 8;
	static const DWORD kColorBits = kColorChannelBits * 4;
	static const DWORD kDepthBits = 24;
	static const DWORD kStencilBits = 8;
	static const DWORD kDepthMemoryBits = 32;

	static RenderContext* sSharedRenderContext = nullptr;
	static HWND sSharedRenderWnd = nullptr;
	static LRESULT CALLBACK SharedRenderContextProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
		switch (msg){
		case WM_ERASEBKGND:
			return 1;
		default:
			break;
		}
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	RenderContext* GetSharedRenderContext()
	{
		return sSharedRenderContext;
	}
	static GLuint _glewStrCLen(const GLubyte* s, GLubyte c)
	{
		GLuint i = 0;
		if (s == NULL) return 0;
		while (s[i] != '\0' && s[i] != c) i++;
		return (s[i] == '\0' || s[i] == c) ? i : 0;
	}
	bool CreateSharedRenderContext()
	{
		sSharedRenderContext = new RenderContext;
		BaseWindow::RegisterWindowClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC,L"SharedRenderContextProc",SharedRenderContextProc);
		sSharedRenderWnd = CreateWindow(
			L"SharedRenderContextProc",
			L"AliceHiddenWindow",
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			0, 0, 32, 32,
			NULL, NULL,
			BaseWindow::mAppInstance, NULL);
		HDC hdc = GetDC(sSharedRenderWnd);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32; // 8 bits stencil buffer
		pfd.cDepthBits = 24; // 24 bits zbuffer
		pfd.cStencilBits = 8; // 8 bits stencil buffer
		pfd.iLayerType = PFD_MAIN_PLANE; // main layer
		int pixelformat = ChoosePixelFormat(hdc, &pfd);
		if (pixelformat == 0)
		{
			Error("choose pixel format fail %d", pixelformat);
			return false;
		}

		if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
		{
			Error("pixel format is %d", pixelformat);
			return false;
		}
		wglMakeCurrent(NULL, NULL);
		HGLRC rc = wglCreateContext(hdc);
		if (rc == NULL)
		{
			Error("create GL RC fail!\n");
			return false;
		}
		if (!wglMakeCurrent(hdc, rc))
		{
			Error("GLContext: failed to activate context\n");
			ReleaseDC(sSharedRenderWnd, hdc);
			return false;
		}
		const GLubyte* s;
		GLuint dot;
		GLint major, minor;
		/* query opengl version */
		s = glGetString(GL_VERSION);
		dot = _glewStrCLen(s, '.');
		if (dot == 0)
			return false;

		major = s[dot - 1] - '0';
		minor = s[dot + 1] - '0';

		if (minor < 0 || minor > 9)
			minor = 0;
		if (major < 0 || major>9)
			return false;

		if (major < 2)
		{
			Error("opengl version %d < 2",major);
			return false;
		}
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			Error("glew init fail");
			return false;
		}
		sSharedRenderContext->SetRC(rc);
		sSharedRenderContext->SetDC(hdc);
		if (!wglChoosePixelFormatARB)
		{
			// We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
			printf("OpenGL Display driver does not support wglChoosePixelFormatARB function\n");
			return false;
		}
		else
		{
			//printf("OpenGL Display driver support wglChoosePixelFormatARB function\n");
			wglMakeCurrent(hdc, NULL);
			wglDeleteContext(rc);
			rc = nullptr;
			ReleaseDC(sSharedRenderWnd, hdc);
			hdc = nullptr;
			DestroyWindow(sSharedRenderWnd);

			sSharedRenderWnd = CreateWindowEx(WS_EX_TOOLWINDOW, L"SharedRenderContextProc", L"AliceGLWindow",
				WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				0, 0, 32, 32,
				NULL, NULL, BaseWindow::mAppInstance, NULL);

			if (!sSharedRenderWnd)
			{
				Error("create render window fail %d 2", GetLastError());
			}
			hdc = GetDC(sSharedRenderWnd);
			sSharedRenderContext->SetDC(hdc);
			wglMakeCurrent(NULL, NULL);
			int numSamples = 1;
			int wglpfd[] = {
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_RED_BITS_ARB,       kColorChannelBits,
				WGL_GREEN_BITS_ARB,     kColorChannelBits,
				WGL_BLUE_BITS_ARB,      kColorChannelBits,
				WGL_ALPHA_BITS_ARB,     kColorChannelBits,
				WGL_DEPTH_BITS_ARB,     kDepthBits,
				WGL_STENCIL_BITS_ARB,   kStencilBits,
				0,0
			};
			int pixelFormat[256];
			UINT numFormat;
			if (!wglChoosePixelFormatARB(hdc, wglpfd, NULL, 256, pixelFormat, &numFormat) && numFormat == 0)
			{
				Error("wglChoosePixelFormatARB(hdc, wglpfd, fAttris, 1, &pixelFormat, &numFormat) fail");
				return false;
			}
			//Info("wglChoosePixelFormatARB total %d",  numFormat);
			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory(&pfd, sizeof(pfd));

			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
			pfd.iPixelType = PFD_TYPE_RGBA;

			pfd.cColorBits = 32; // 24 bits zbuffer
			pfd.cDepthBits = 24; // 24 bits zbuffer
			pfd.cStencilBits = 8; // 8 bits stencil buffer
			pfd.iLayerType = PFD_MAIN_PLANE; // main layer
			DescribePixelFormat(hdc, pixelFormat[0], sizeof(pfd), &pfd);
			if (pixelFormat[0] == 0)
			{
				Error("choose pixel format fail %d total %d", pixelFormat[0], numFormat);
				return false;
			}
			if (SetPixelFormat(hdc, pixelFormat[0], &pfd) == FALSE)
			{
				Error("SetPixelFormat fail %d total %d", pixelFormat[0], numFormat);
				return false;
			}

			HGLRC rc = wglCreateContext(hdc);
			wglMakeCurrent(hdc, rc);
			sSharedRenderContext->SetRC(rc);
			sSharedRenderContext->SetDC(hdc);

		}
		return true;
	}

	void RenderContext::SetRC(HGLRC rc)
	{
		mRC = rc;
	}

	void RenderContext::ShareResourceFromSharedContext()
	{
		wglShareLists(sSharedRenderContext->GetRC(), mRC);
	}

	HGLRC RenderContext::GetRC()
	{
		return mRC;
	}

	void RenderContext::SetDC(HDC hdc)
	{
		mDC = hdc;
	}

	void RenderContext::Bind()
	{
		if (wglGetCurrentContext() == mRC)
		{
			return;
		}
		if (!wglMakeCurrent(mDC, mRC))
		{
			Error("wglMakeCurrent fail %p %p %d",mDC,mRC,GetLastError());
		}
	}

	void RenderContext::UnBind()
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			Error("fail to unbind %p %p %d",mDC,mRC,GetLastError());
		}
	}

	RenderContext* RenderContext::Create()
	{
		return nullptr;
	}
}