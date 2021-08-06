#include "ScaledImage.h"
#include "Runtime/String/StringUtils.h"

namespace Editor
{
	ScaledImage::ScaledImage():mImageData(nullptr),mType(ScaledImageTypeNormal)
	{
		mImageDataStreamImageData[0] = nullptr;
		mImageDataStreamImageData[1] = nullptr;
		mImageDataStreamImageData[2] = nullptr;
	}
	
	void ScaledImage::SetRect(Rect&rect)
	{
		if (rect.Width==0)
		{
			mRect.X = rect.X;
			mRect.Y = rect.Y;
			mRect.Width = mImageData->GetWidth();
			mRect.Height = mImageData->GetHeight();
		}
		else
		{
			mRect = rect;
		}
	}

	void ScaledImage::SetRect(int x, int y, int width, int height)
	{
		mRect.X = x;
		mRect.Y = y;
		if (width == 0||height==0)
		{
			width = mImageData->GetWidth();
			height = mImageData->GetHeight();
		}
		mRect.Width = width;
		mRect.Height = height;
	}

	void ScaledImage::Draw(Graphics&painter)
	{
		switch (mType)
		{
		case Editor::ScaledImage::ScaledImageTypeNormal:
		{
			painter.DrawImage(mImageData, mRect);
		}
			break;
		case Editor::ScaledImage::ScaledImageTypeH3:
			DrawH3(painter);
			break;
		case Editor::ScaledImage::ScaledImageTypeV3:
			DrawV3(painter);
			break;
		case Editor::ScaledImage::ScaledImageSplit9:
			DrawS9(painter);
			break;
		case Editor::ScaledImage::ScaledImageTypeCompositeImageH3:
			DrawCompositeImageH3(painter);
			break;
		case Editor::ScaledImage::ScaledImageTypeCompositeImageV3:
			DrawCompositeImageV3(painter);
			break;
		}
		Control::Draw(painter);
	}

	void ScaledImage::SetImageData(Image*imageData)
	{
		mImageData = imageData;
	}

	void ScaledImage::SetSplitH3(int left, int middle)
	{
		mSplitRect.X = left;
		mSplitRect.Width = middle;
		mType = ScaledImageTypeH3;
	}

	void ScaledImage::SetSplitV3(int top, int middle)
	{
		mSplitRect.Y = top;
		mSplitRect.Height = middle;
		mType = ScaledImageTypeV3;
	}

	void ScaledImage::SetSplitS9(Rect&rect)
	{
		mSplitRect = rect;
		mType = ScaledImageSplit9;
	}

	void ScaledImage::DrawH3(Graphics&painter)
	{
		int middleLen = mRect.Width - (mImageData->GetWidth()-mSplitRect.X-mSplitRect.Width)-mSplitRect.X;
		int rightLen = mImageData->GetWidth() - mSplitRect.X - mSplitRect.Width;
		//left
		painter.DrawImage(mImageData, Rect(mRect.X, mRect.Y, mSplitRect.X, mRect.Height), 0, 0, mSplitRect.X, mImageData->GetHeight(), Gdiplus::Unit::UnitPixel);
		//middle
		painter.DrawImage(mImageData, Rect(mRect.X +mSplitRect.X , mRect.Y, middleLen, mRect.Height), mSplitRect.X, 0, mSplitRect.Width, mImageData->GetHeight(), Gdiplus::Unit::UnitPixel);
		////right
		painter.DrawImage(mImageData, Rect(mRect.X+ mSplitRect.X + middleLen, mRect.Y, rightLen, mRect.Height), mSplitRect.X + mSplitRect.Width, 0, rightLen, mImageData->GetHeight(), Gdiplus::Unit::UnitPixel);
		//Debug("%d=%d+%d+%d",mRect.Width,mSplitRect.X,middleLen,rightLen);
	}

	void ScaledImage::DrawV3(Graphics&painter)
	{
		int upper = mSplitRect.Y;
		int middle = mRect.Height - upper * 2;
		//upper
		painter.DrawImage(mImageData, Rect(mRect.X, mRect.Y, mRect.Width, upper), 0, 0, mRect.Width, upper, Gdiplus::Unit::UnitPixel);
		//middle
		painter.DrawImage(mImageData, Rect(mRect.X, mRect.Y+upper, mRect.Width,middle), 0, upper, mRect.Width,mImageData->GetHeight()- 2*upper, Gdiplus::Unit::UnitPixel);
		//bottom
		painter.DrawImage(mImageData, Rect(mRect.X , mRect.Y+upper+middle, mRect.Width, upper), 0, mImageData->GetHeight()-upper, mRect.Width, upper, Gdiplus::Unit::UnitPixel);
	}

	void ScaledImage::DrawS9(Graphics&painter)
	{
		//left top
		painter.DrawImage(mImageData, Rect(mRect.X, mRect.Y, mSplitRect.X, mSplitRect.Y),0,0,mSplitRect.X,mSplitRect.Y,UnitPixel);
		//middle top
		painter.DrawImage(mImageData, Rect(mRect.X+mSplitRect.X, mRect.Y, mRect.Width-mSplitRect.X-(mImageData->GetWidth()-mSplitRect.X-mSplitRect.Width), mSplitRect.Y),
			mSplitRect.X, 0, mSplitRect.Width, mSplitRect.Y, UnitPixel);
		//right top
		painter.DrawImage(mImageData, Rect(mRect.Width - (mImageData->GetWidth() - mSplitRect.X - mSplitRect.Width)-mRect.X, mRect.Y, mImageData->GetWidth() - mSplitRect.X - mSplitRect.Width, mSplitRect.Y),
			mSplitRect.X+mSplitRect.Width, 0, (mImageData->GetWidth() - mSplitRect.X - mSplitRect.Width), mSplitRect.Y, UnitPixel);
		//left middle 
		painter.DrawImage(mImageData, Rect(mRect.X, mRect.Y+mSplitRect.Y, mSplitRect.X, mRect.Height-mSplitRect.Y-(mImageData->GetHeight()-mSplitRect.Y-mSplitRect.Height)),
			0, mSplitRect.Y, mSplitRect.X, mSplitRect.Height, UnitPixel);
		//center
		painter.DrawImage(mImageData, Rect(mRect.X + mSplitRect.X, mRect.Y + mSplitRect.Y, 1000+mRect.Width - mSplitRect.X - (mImageData->GetWidth() - mSplitRect.X - mSplitRect.Width), mRect.Height - mSplitRect.Y - (mImageData->GetHeight() - mSplitRect.Y - mSplitRect.Height)),
			mSplitRect.X, mSplitRect.Y, mSplitRect.Width, mSplitRect.Height, UnitPixel);
		//right middle
		//left bottom
		//middle bottom
		//right bottom
	}

	void ScaledImage::SetCompositeImageH3(const char*left, const char*middle, const char*right)
	{
		mImageDataStreamImageData[0] = ImageControl::LoadImageDataFromAssetsPath(left);
		mImageDataStreamImageData[1] = ImageControl::LoadImageDataFromAssetsPath(middle);
		mImageDataStreamImageData[2] = ImageControl::LoadImageDataFromAssetsPath(right);
		mImageData = mImageDataStreamImageData[0]->mImage;
		mImageData2 = mImageDataStreamImageData[1]->mImage;
		mImageData3 = mImageDataStreamImageData[2]->mImage;
		mType = ScaledImageTypeCompositeImageH3;
	}

	void ScaledImage::SetCompositeImageH3(Image*left, Image*middle, Image*right)
	{
		mImageData = left;
		mImageData2 = middle;
		mImageData3 = right;
		mType = ScaledImageTypeCompositeImageH3;
	}

	void ScaledImage::SetCompositeImageV3(const char*upper, const char*middle, const char*bottom)
	{
		mImageDataStreamImageData[0] = ImageControl::LoadImageDataFromAssetsPath(upper);
		mImageDataStreamImageData[1] = ImageControl::LoadImageDataFromAssetsPath(middle);
		mImageDataStreamImageData[2] = ImageControl::LoadImageDataFromAssetsPath(bottom);
		mImageData = mImageDataStreamImageData[0]->mImage;
		mImageData2 = mImageDataStreamImageData[1]->mImage;
		mImageData3 = mImageDataStreamImageData[2]->mImage;
		mType = ScaledImageTypeCompositeImageV3;
	}

	void ScaledImage::SetCompositeImageV3(Image*upper, Image*middle, Image*bottom)
	{
		mImageData = upper;
		mImageData2 = middle;
		mImageData3 = bottom;
		mType = ScaledImageTypeCompositeImageV3;
	}

	void ScaledImage::DrawCompositeImageH3(Graphics&painter)
	{
		//left
		painter.DrawImage(mImageData, mRect.X, mRect.Y,mImageData->GetWidth(),mImageData->GetHeight());
		//middle
		painter.DrawImage(mImageData2, mRect.X + mImageData->GetWidth(), mRect.Y, mRect.Width-mImageData->GetWidth()-mImageData3->GetWidth(), mImageData2->GetHeight());
		//right
		painter.DrawImage(mImageData3, mRect.X +  mRect.Width - mImageData3->GetWidth(), mRect.Y, mImageData3->GetWidth(), mImageData3->GetHeight());
	}

	void ScaledImage::DrawCompositeImageV3(Graphics&painter)
	{
		//middle
		//painter.DrawImage(mImageData2, mRect.X, mRect.Y, mRect.Width, mRect.Height);
		//upper
		painter.DrawImage(mImageData, mRect.X, mRect.Y, mRect.Width, mImageData->GetHeight());
		//bottom
		painter.DrawImage(mImageData3, mRect.X, mRect.Y+(mRect.Height-mImageData3->GetHeight()), mRect.Width, mImageData3->GetHeight());
	}

	Image*ScaledImage::GetImage1()
	{
		return mImageData;
	}

	ScaledImage::~ScaledImage()
	{
		for (int i = 0; i < 3; i++)
		{
			if (mImageDataStreamImageData[i] != nullptr)
			{
				delete mImageDataStreamImageData[i];
			}
		}
	}
}