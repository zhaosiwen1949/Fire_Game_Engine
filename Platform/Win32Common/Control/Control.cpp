#include "Control.h"

namespace Editor
{
	Control::Control():mNext(nullptr),mPrev(nullptr)
	{

	}
	void Control::PushBack(Control*control)
	{
		if (mNext==nullptr)
		{
			mNext = control;
			control->mPrev = this;
			return;
		}
		mNext->PushBack(control);
	}

	void Control::Remove(Control*control)
	{
		if (mNext == control)
		{
			mNext = mNext->mNext;
			mNext->mNext->mPrev = this;
			return;
		}
		mNext->Remove(control);
	}

	bool Control::TestIntersect(int x, int y)
	{
		if (mRect.Contains(x,y))
		{
			mIntersectPos = ControlIntersectPosMiddle;
			if (y <= mRect.Y + 2)
			{
				mIntersectPos = ControlIntersectPosUpper;
			}
			else if (y >= mRect.Y + mRect.Height - 2)
			{
				mIntersectPos = ControlIntersectPosBottom;
			}
			return true;
		}
		return false;
	}

	Control*Control::Intersect(int x, int y)
	{
		if (TestIntersect(x,y))
		{
			return this;
		}
		if (mNext!=nullptr)
		{
			return mNext->Intersect(x, y);
		}
		return nullptr;
	}

	void Control::Draw(Graphics&painter)
	{
		if (mNext!=nullptr)
		{
			mNext->Draw(painter);
		}
	}

	void Control::SetRect(Rect &rect)
	{
		mRect = rect;
	}

	void Control::SetPos(int x, int y)
	{
		mRect.X = x;
		mRect.Y = y;
	}

	void Control::SetRect(int x, int y, int width, int height)
	{
		mRect.X = x;
		mRect.Y = y;
		mRect.Width = width;
		mRect.Height = height;
	}

	Rect & Control::GetRect()
	{
		return mRect;
	}
}