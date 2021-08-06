#include "SelectableImageControl.h"

namespace Editor
{
	ImageAttributes*SelectableImageControl::mImageAttributeWhenSelected = nullptr;
	SelectableImageControl::SelectableImageControl():mbSelected(false),mShowSelectedMode(kShowSelectedModeAddColor),mOnClickHandler(nullptr)
	{
		mBKGColor = Color(0,100,150);
		if (mImageAttributeWhenSelected==nullptr)
		{
			mImageAttributeWhenSelected = new ImageAttributes;
			ColorMatrix colorMatrix = 
			{
				1.4f,0.0f,0.0f,0.0f,0.0f,
				0.0f,1.4f,0.0f,0.0f,0.0f,
				0.0f,0.0f,1.4f,0.0f,0.0f,
				0.0f,0.0f,0.0f,1.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,1.0f
			};
			mImageAttributeWhenSelected->SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
		}
	}

	void SelectableImageControl::Select()
	{
		mbSelected = true;
	}

	void SelectableImageControl::ResetState()
	{
		if (mbSelected)
		{
			mbSelected=false;
		}
		if (mNext != nullptr)
		{
			((SelectableImageControl*)mNext)->ResetState();
		}
	}

	Control* SelectableImageControl::OnClickDown(int x, int y)
	{
		if (TestIntersect(x, y))
		{
			return this;
		}
		if (mNext != nullptr)
		{
			return ((SelectableImageControl*)mNext)->OnClickDown(x, y);
		}
		return nullptr;
	}

	Control* SelectableImageControl::OnClickUp(int x, int y)
	{
		if (TestIntersect(x, y))
		{
			mbSelected = true;
			
			if (mOnClickHandler != nullptr)
			{
				mOnClickHandler();
			}
			return this;
		}
		if (mNext != nullptr)
		{
			return ((SelectableImageControl*)mNext)->OnClickUp(x, y);
		}
		return nullptr;
	}

	void SelectableImageControl::SetOnClick(std::function<void()>onClicked)
	{
		mOnClickHandler = onClicked;
	}

	void SelectableImageControl::Unselect()
	{
		mbSelected = false;
	}

	bool SelectableImageControl::IsSelected()
	{
		return mbSelected;
	}

	void SelectableImageControl::SetShowSelectedMode(ShowSelectedMode selectedMode)
	{
		mShowSelectedMode = selectedMode;
	}

	void SelectableImageControl::SetBKGColor(Color&color)
	{
		mBKGColor = color;
	}

	void SelectableImageControl::Draw(Graphics&painter)
	{
		if (mImageData != nullptr)
		{
			switch (mRenderMode)
			{
			case Editor::ImageRenderModeFill:
			{
				if (mbSelected)
				{
					if (mShowSelectedMode == kShowSelectedModeAddColor)
					{
						painter.DrawImage(mImageData, mRect, 0, 0, mImageData->GetWidth(), mImageData->GetHeight(), UnitPixel, mImageAttributeWhenSelected);
					}
					else
					{
						SolidBrush brush(mBKGColor);
						painter.FillRectangle(&brush, mRect);
					}
				}
				else
				{
					painter.DrawImage(mImageData, mRect, 0, 0, mImageData->GetWidth(), mImageData->GetHeight(), UnitPixel);
				}
			}
			break;
			case Editor::ImageRenderModeAudoAdjust:
			{
				UINT width = mImageData->GetWidth();
				UINT height = mImageData->GetHeight();
				Rect dstRect;
				if (width != height)
				{
					if (width > height)
					{
						INT rh = (INT)(mRect.Height * height / width);
						dstRect.X = mRect.X;
						dstRect.Y = mRect.Y + (mRect.Height - rh) / 2;
						dstRect.Width = mRect.Width;
						dstRect.Height = rh;
						//painter.DrawImage(mImageData, mRect.X, mRect.Y + (mRect.Height - rh) / 2, mRect.Width, rh);
					}
					else//width < height
					{
						INT rw = (INT)(mRect.Width * width / height);
						dstRect.X = mRect.X + (mRect.Width - rw) / 2;
						dstRect.Y = mRect.Y;
						dstRect.Width = rw;
						dstRect.Height = mRect.Height;
						//painter.DrawImage(mImageData, mRect.X + (mRect.Width - rw) / 2, mRect.Y, rw, mRect.Height);
					}
				}
				else
				{
					dstRect = mRect;
					//painter.DrawImage(mImageData, mRect, 0, 0, mImageData->GetWidth(), mImageData->GetHeight(), UnitPixel);
				}
				if (mbSelected)
				{
					if (mShowSelectedMode==kShowSelectedModeAddColor)
					{
						painter.DrawImage(mImageData, dstRect, 0, 0, width, height, UnitPixel, mImageAttributeWhenSelected);
					}
					else
					{
						SolidBrush brush(mBKGColor);
						painter.FillRectangle(&brush, mRect); 
						painter.DrawImage(mImageData, dstRect, 0, 0, width, height, UnitPixel);
					}
				}
				else
				{
					painter.DrawImage(mImageData, dstRect, 0, 0, width, height, UnitPixel);
				}
			}
			break;
			case ImageRenderModeCenterNoScale:
			{
				UINT width = mImageData->GetWidth();
				UINT height = mImageData->GetHeight();
				Rect dstRect;

				dstRect.X = mRect.X + (mRect.Width - width) / 2;
				dstRect.Y = mRect.Y + (mRect.Height - height) / 2;
				dstRect.Width = width;
				dstRect.Height = height;

				if (mbSelected)
				{
					if (mShowSelectedMode == kShowSelectedModeAddColor)
					{
						painter.DrawImage(mImageData, dstRect, 0, 0, width, height, UnitPixel, mImageAttributeWhenSelected);
					}
					else
					{
						SolidBrush brush(mBKGColor);
						painter.FillRectangle(&brush, mRect);
					}
				}
				painter.DrawImage(mImageData, dstRect, 0, 0, width, height, UnitPixel);
			}
			break;
			}
		}
		Control::Draw(painter);
	}
}