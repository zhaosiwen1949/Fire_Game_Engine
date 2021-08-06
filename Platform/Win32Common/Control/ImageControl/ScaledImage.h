#pragma once
#include "ImageControl.h"

namespace Editor
{
	class ScaledImage:public Control
	{
	public:
		enum ScaledImageType
		{
			ScaledImageTypeNormal,
			ScaledImageTypeH3,
			ScaledImageTypeV3,
			ScaledImageSplit9,
			ScaledImageTypeCompositeImageH3,
			ScaledImageTypeCompositeImageV3
		};
	public:
		ScaledImage();
		~ScaledImage();
		void SetImageData(Image*imageData);
		//left split pos and middle length
		void SetSplitH3(int left,int middle);
		//top split pos and middle length
		void SetSplitV3(int top,int middle);
		//middle rect
		void SetSplitS9(Rect&rect);
		//
		void SetCompositeImageH3(const char*left,const char*middle,const char*right);
		void SetCompositeImageH3(Image*upper, Image*middle, Image*bottom);
		void SetCompositeImageV3(const char*upper, const char*middle, const char*bottom);
		void SetCompositeImageV3(Image*upper, Image*middle, Image*bottom);

		Image*GetImage1();

		virtual void Draw(Graphics&painter);
		virtual void SetRect(Rect&rect);
		virtual void SetRect(int x,int y,int width,int height);

		void CleanUp();
	protected:
		void DrawH3(Graphics&painter);
		void DrawV3(Graphics&painter);
		void DrawS9(Graphics&painter);
		void DrawCompositeImageH3(Graphics&painter);
		void DrawCompositeImageV3(Graphics&painter);
		
	protected:
		Rect mSplitRect;
		ScaledImageType mType;
		StreamImageData*mImageDataStreamImageData[3];
		Image*mImageData;
		Image*mImageData2;
		Image*mImageData3;
	};
}