#include "RenderWindow.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Debugger/Log.h"

namespace Editor
{
	static const DWORD kColorChannelBits = 8;
	static const DWORD kColorBits = kColorChannelBits * 4;
	static const DWORD kDepthBits = 24;
	static const DWORD kStencilBits = 8;
	static const DWORD kDepthMemoryBits = 32;

	GLWindow::GLWindow():mbViewportChanged(false)
	{

	}

	void GLWindow::OnPaint()
	{
		//do nothing
		mbDirty = true;
	}

	bool GLWindow::BeginRendering()
	{
		if (wglGetCurrentContext() != mRC)
		{
			if (!wglMakeCurrent(mHDC, mRC))
			{
				Error("%s wglMakeCurrent fail %p %p %d", GetWindowName(), mHDC, mRC, GetLastError());
				return false;
			}
		}
		if (mbViewportChanged)
		{
			glViewport(0, 0, mRect.Width, mRect.Height);
			mbViewportChanged = false;
		}
		mbNeedToUpdateGLWindow = true;
		return true;
	}

	void GLWindow::EndRendering()
	{
		glFinish();//force to complete pre commands
		wglMakeCurrent(NULL, NULL);
	}

	GLWindow*GLWindow::mCurrentGLWindow = nullptr;
	bool GLWindow::mbNeedToUpdateGLWindow = false;
	void GLWindow::MakeCurrent()
	{
		mCurrentGLWindow = this;
	}

	void GLWindow::Present()
	{
		if (mbNeedToUpdateGLWindow&&mCurrentGLWindow!=nullptr){
			SwapBuffers(mCurrentGLWindow->GetDC());
			mbNeedToUpdateGLWindow = false;
		}
	}
	
	static ATOM glAtom = 0;
	void GLWindow::InitGLWindow(BaseWindow*parent, int x, int y, int width, int height)
	{
		if (glAtom == 0)
		{
			glAtom = RegisterWindowClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, L"GLWindow", WindowEventProc);
		}
		mhWnd = CreateWindowWithStyle(WindowStyleAuxWindow, x, y, width, height, L"GLWindow", parent->GetHwnd());
		SetWindowLongPtr(mhWnd, GWLP_USERDATA, (LONG_PTR)this);
		mHDC = ::GetDC(mhWnd);
		mParent = parent;
		SetRect(x, y, width, height);
	}

	void GLWindow::SetupSimpleGLContext() {
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nVersion = 1;
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		int pixelFormat = ChoosePixelFormat(mHDC, &pfd);
		SetPixelFormat(mHDC, pixelFormat, &pfd);
		mRC = wglCreateContext(mHDC);
		wglMakeCurrent(mHDC, mRC);
	}

	void GLWindow::SetupDefaultGL() {
		wglMakeCurrent(NULL, NULL);
		GLint attribs[]{
			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
			WGL_RED_BITS_ARB,8,
			WGL_GREEN_BITS_ARB,8,
			WGL_BLUE_BITS_ARB,8,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,24,
			WGL_STENCIL_BITS_ARB,8,
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
			WGL_SAMPLES_ARB,16,
			NULL,NULL
		};
		int pixelFormat[256] = { 0 };
		UINT formatNum = 0;
		wglChoosePixelFormatARB(mHDC, attribs, NULL, 256, pixelFormat, &formatNum);
		printf("support format number is %u\n", formatNum);
		if (formatNum > 0){
			PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(mHDC, pixelFormat[0], sizeof(pfd), &pfd);
			SetPixelFormat(mHDC, pixelFormat[0], &pfd);
			int contexAttributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB,4,
				WGL_CONTEXT_MINOR_VERSION_ARB,5,
				WGL_CONTEXT_FLAGS_ARB,WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB ,
				WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};
			mRC = wglCreateContextAttribsARB(mHDC, nullptr, contexAttributes);
		}
		wglMakeCurrent(mHDC, mRC);
	}
	int GLWindow::MaxSampleBuffer() {
		int buffer_count = 32;
		while (buffer_count>1){
			GLint attribs[]{
				WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
				WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
				WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
				WGL_RED_BITS_ARB,8,
				WGL_GREEN_BITS_ARB,8,
				WGL_BLUE_BITS_ARB,8,
				WGL_ALPHA_BITS_ARB,8,
				WGL_DEPTH_BITS_ARB,24,
				WGL_STENCIL_BITS_ARB,8,
				WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
				WGL_SAMPLES_ARB,buffer_count,
				NULL,NULL
			};
			int pixelFormat[256] = { 0 };
			UINT formatNum = 0;
			wglChoosePixelFormatARB(mHDC, attribs, NULL, 256, pixelFormat, &formatNum);
			if (formatNum>0){
				return buffer_count;
			}
			buffer_count /= 2;
		}
		return 1;
	}
}