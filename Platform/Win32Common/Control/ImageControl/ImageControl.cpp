#include "ImageControl.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/Allocator/DefaultAllocator.h"

namespace Editor
{
	ImageControl::ImageControl():mbUseSharedImage(true),mImageData(nullptr),mRenderMode(ImageRenderModeFill), mImageMemory(nullptr)
	{

	}

	ImageControl::~ImageControl()
	{
		if (mImageMemory!=nullptr)
		{
			delete mImageMemory->mImage;
			mImageMemory->mStream->Release();
		}
	}

	Image* ImageControl::SetImageData(Image*data)
	{
		Image*ret = mImageData;
		mImageData = data;
		mbUseSharedImage = true;
		return ret;
	}
	
	Image*ImageControl::GetImageData()
	{
		return mImageData;
	}

	Image* ImageControl::SetImagePath(const char*imagePath)
	{
		Image*ret = mImageData;
		if (mImageMemory != nullptr)
		{
			delete mImageMemory->mImage;
			mImageMemory->mStream->Release();
		}
		mImageMemory = LoadImageDataFromAssetsPath(imagePath);
		mImageData = mImageMemory->mImage;
		mbUseSharedImage = false;
		return ret;
	}

	void ImageControl::SetRenderMode(ImageRenderMode mode)
	{
		mRenderMode = mode;
	}

	void ImageControl::SetRect(int x, int y, int width, int height)
	{
		if (width == 0||height==0)
		{
			if (mImageData!=nullptr)
			{
				width = mImageData->GetWidth();
				height = mImageData->GetHeight();
			}
		}
		Control::SetRect(x, y, width, height);
	}

	void ImageControl::Draw(Graphics&painter)
	{
		if (mImageData != nullptr)
		{
			switch (mRenderMode)
			{
			case Editor::ImageRenderModeFill:
			{
				painter.DrawImage(mImageData, mRect, 0, 0, mImageData->GetWidth(), mImageData->GetHeight(), UnitPixel);
			}
			break;
			case Editor::ImageRenderModeAudoAdjust:
			{
				UINT width = mImageData->GetWidth();
				UINT height = mImageData->GetHeight();
				if (width != height)
				{
					if (width > height)
					{
						INT rh = (INT)(mRect.Height * height / width);
						painter.DrawImage(mImageData, mRect.X, mRect.Y + (mRect.Height - rh) / 2, mRect.Width, rh);
					}
					else//width < height
					{
						INT rw = (INT)(mRect.Width * width / height);
						painter.DrawImage(mImageData, mRect.X + (mRect.Width - rw) / 2, mRect.Y, rw, mRect.Height);
					}
				}
				else
				{
					painter.DrawImage(mImageData, mRect, 0, 0, mImageData->GetWidth(), mImageData->GetHeight(), UnitPixel);
				}
			}
			break;
			case Editor::ImageRenderModeCenterNoScale:
			{
				int width = mImageData->GetWidth();
				int height = mImageData->GetHeight();
				int xPos = mRect.X + mRect.Width / 2 - width / 2;
				int yPos = mRect.Y + mRect.Height / 2 - height / 2;
				painter.DrawImage(mImageData, xPos, yPos,width,height);
			}
				break;
			default:
				break;
			}
		}
		Control::Draw(painter);
	}

	StreamImageData*ImageControl::LoadImageDataFromAssetsPath(const char*path)
	{
		using namespace Alice;
		Data data;
		if (Alice::FileSystem::LoadDataFromPath(path, data))
		{
			HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, data.mDataLen);
			BYTE* pMem = (BYTE*)GlobalLock(hMem);
			memcpy(pMem, data.mData, data.mDataLen);
			IStream *stream;
			HRESULT hr = CreateStreamOnHGlobal(pMem, TRUE, &stream);
			Image*image = Gdiplus::Image::FromStream(stream);
			GlobalUnlock(hMem);
			return  new(kMemEditorId)StreamImageData(stream, image);
		}
		return nullptr;
	}
}