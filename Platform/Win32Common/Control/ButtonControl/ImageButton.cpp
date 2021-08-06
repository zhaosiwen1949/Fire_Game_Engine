#include "ImageButton.h"

namespace Editor
{
	ImageButton::ImageButton():mOnClickHandler(nullptr)
	{
		mOriginalPos.X = -1;
		mbIsDown = false;
	}

	void ImageButton::ResetState()
	{
		if (mbIsDown)
		{
			mbIsDown = false;
			mRect.X = mOriginalPos.X;
			mRect.Y = mOriginalPos.Y;
			mOriginalPos.X = -1;
		}
		if (mNext != nullptr)
		{
			((ImageButton*)mNext)->ResetState();
		}
	}
	bool ImageButton::IsDown() { return mbIsDown; }
	Control* ImageButton::OnClickDown(int x, int y)
	{
		if (TestIntersect(x,y))
		{
			mOriginalPos.X = mRect.X;
			mOriginalPos.Y = mRect.Y;
			mRect.Y += 2;
			mbIsDown = true;
			return this;
		}
		if (mNext!=nullptr)
		{
			return ((ImageButton*)mNext)->OnClickDown(x,y);
		}
		return nullptr;
	}

	Control* ImageButton::OnClickUp(int x, int y)
	{
		if (TestIntersect(x, y))
		{
			if (mOnClickHandler!=nullptr)
			{
				mOnClickHandler();
			}
			return this;
		}
		if (mNext != nullptr)
		{
			return ((ImageButton*)mNext)->OnClickUp(x, y);
		}
		return nullptr;
	}

	void ImageButton::SetOnClick(std::function<void()>onClicked)
	{
		mOnClickHandler = onClicked;
	}
}