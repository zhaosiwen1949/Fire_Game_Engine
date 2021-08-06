#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Plugins/freeimage/FreeImage.h"
#include "Runtime/IO/AliceData.h"
namespace Alice{
	int RawImage2Uncompressed32BITRGBA(unsigned char * const data, int dataLen, Alice::Data&outPixel, int&width, int&height);
	bool GetImageInfo(unsigned char * const data, int dataLen,int&width, int&height,int&bpp);
	bool SaveFreeTypeFontToPNG32(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height);
	bool SaveRGBAToPNG32(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height);
	bool SaveRGBToJPEG24(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height);
	bool SaveRToJPEG8(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height);
	void BGRAToRGBA(unsigned char * pixel, int width, int height);
	void BGRToRGB(unsigned char * pixel, int width, int height);
	void AlphaToRGBA(unsigned char luminance,unsigned char * pixel, int width, int height, unsigned char *out);
	AliceAny GenAlphaGradiantQuadImage(int size,float e=8.0f);
	AliceAny GenRGBImage(int width, int height, AliceUInt8 r,AliceUInt8 g,AliceUInt8 b);
	class ImageUtils{
	public:
		static void Init();
		static void OnImageOperationException(FREE_IMAGE_FORMAT fif, const char *message);
	};
}