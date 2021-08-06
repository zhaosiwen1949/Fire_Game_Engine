#pragma once
#include "BaseWindow.h"
#include "Runtime/Render/AliceGL.h"
namespace Editor{
	//to create this in main thread
	class RenderContext{
	protected:
		HDC mDC;
		HGLRC mRC;
	public:
		void SetDC(HDC hdc);
		void SetRC(HGLRC rc);
		void ShareResourceFromSharedContext();
		HGLRC GetRC();
		void Bind();
		void UnBind();
	public:
		static RenderContext* Create();
	};
	bool CreateSharedRenderContext();
	RenderContext* GetSharedRenderContext();
}