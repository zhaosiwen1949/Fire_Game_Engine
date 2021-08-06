#pragma once
#include "AliceResource.h"
#include "AliceResourceType.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"
namespace Alice{
	class ImageResource :public Resource {
	public:
		ImageResource();
		~ImageResource();
		Data mPixelData;
		int mWidth, mHeight;
		int mBPP;
		static ImageResource* Init(const char *filepath);
		bool InitWithRawImageFileData(unsigned char * const data, int dataLen);//create from png/bmp...
		bool InitWithTextureAssetData(const unsigned char*data, int dataLen);//create from alice asset
		void Save(const char*filePath);
		DECLEAR_ALICE_CLASS(ImageResource)
	}; 
}