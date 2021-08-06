#pragma once
#include "BaseWindow.h"
namespace Editor {
	class DoubleBufferedWindow :public BaseWindow {
	protected:
		HDC mBKGDC;
		HBITMAP mBKGBMP;
		int mBufferWidth, mBufferHeight;
	protected:
		virtual void OnPaint();
		virtual void OnEndPaint();
	public:
		BaseWindow * mChild;
		DoubleBufferedWindow();
		void InitDoubleBuffer();
	};
}