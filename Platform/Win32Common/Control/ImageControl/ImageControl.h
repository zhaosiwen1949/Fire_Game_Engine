#pragma once
#include "Platform/Win32Common/Control/Control.h"
namespace Editor
{
	enum ImageRenderMode
	{
		ImageRenderModeFill,//fill the full rect of the control
		ImageRenderModeAudoAdjust,//auto scale width & height
		ImageRenderModeCenterNoScale//no width & height
	};

	struct StreamImageData
	{
		StreamImageData(IStream*stream,Image*image):mStream(stream),mImage(image)
		{

		}
		~StreamImageData()
		{
			if (mImage!=nullptr)
			{
				delete mImage;
			}
			if (mStream != nullptr)
			{
				mStream->Release();
			}
		}
		IStream *mStream;
		Image*mImage;
	};

	class ImageControl :public Control 
	{
	protected:
		Image*mImageData;
		ImageRenderMode mRenderMode;
		bool mbUseSharedImage;
		StreamImageData*mImageMemory;
	public:
		std::string mImagePath;
		ImageControl();
		~ImageControl();
		void SetRenderMode(ImageRenderMode mode);
		Image* SetImageData(Image*data);
		Image* SetImagePath(const char*imagePath);
		Image*GetImageData();
		static StreamImageData*LoadImageDataFromAssetsPath(const char*path);
	public:
		virtual void Draw(Graphics&painter);
		virtual void SetRect(int x,int y,int width,int height);
	};
}