#pragma once
#include "Runtime/RuntimePrefix.h"
namespace Editor{
	enum ControlIntersectPos{
		ControlIntersectPosUpper,
		ControlIntersectPosMiddle,
		ControlIntersectPosBottom
	};
	class Control
	{
	public:
		Control();
		void PushBack(Control*control);
		void Remove(Control*control);
		virtual bool TestIntersect(int x,int y);
		Control*Intersect(int x, int y);
		ControlIntersectPos mIntersectPos;
	public:
		virtual void Draw(Graphics&painter);
		virtual void SetRect(Rect &rect);
		virtual void SetPos(int x,int y);
		virtual void SetRect(int x, int y, int width, int height);
		virtual Rect&GetRect();
	protected:
		Rect mRect;
	public:
		Control*mNext;
		Control*mPrev;
	};
}