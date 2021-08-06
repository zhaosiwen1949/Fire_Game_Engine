#pragma once
#include "BaseWindow.h"
#include "RenderContext.h"
namespace Editor{
	class GLWindow :public BaseWindow{
	protected:
		HGLRC mRC;
		bool mbViewportChanged;
	protected:
		void OnPaint();
	public:
		GLWindow();
		bool BeginRendering();
		void EndRendering();
		void SetupDefaultGL();
		void SetupSimpleGLContext();
		void InitGLWindow(BaseWindow*parent,int x,int y,int width,int height);
		int MaxSampleBuffer();
	public:
		void MakeCurrent();
		static GLWindow* mCurrentGLWindow;
		static bool mbNeedToUpdateGLWindow;
		static void Present();
	};

}