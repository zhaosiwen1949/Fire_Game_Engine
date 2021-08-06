#include "ImageUtils.h"
#include "Runtime/Plugins/freeimage/FreeImage.h"
#include "Runtime/Debugger/Log.h"
#include <fstream>
#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"FreeImaged.lib")
#pragma comment(lib,"libzxing-debug.lib")
#else
#endif
#endif
namespace Alice{
	void ImageUtils::Init(){
		FreeImage_SetOutputMessage(OnImageOperationException);
	}
	void ImageUtils::OnImageOperationException(FREE_IMAGE_FORMAT fif, const char *message){
		Error("\n*** ");
		if (fif != FIF_UNKNOWN) {
			Error("%s Format\n", FreeImage_GetFormatFromFIF(fif));
		}
#if ALICE_ANDROID
#else
		Error(message);
#endif
	}
	AliceAny GenAlphaGradiantQuadImage(int size, float e) {
		unsigned char *imageData = new unsigned char[size*size * 4];
		float halfSize = (float)size / 2.0f;
		float maxDistance = sqrtf(halfSize*halfSize + halfSize * halfSize);
		float centerX = halfSize;
		float centerY = halfSize;
		for (int y = 0; y < size; ++y) {
			for (int x = 0; x < size; ++x) {
				int currentPixelOffset = (x + y * size) * 4;
				imageData[currentPixelOffset] = 255;
				imageData[currentPixelOffset + 1] = 255;
				imageData[currentPixelOffset + 2] = 255;
				float deltaX = (float)x - centerX;
				float deltaY = (float)y - centerY;
				float distance = sqrtf(deltaX*deltaX + deltaY * deltaY);
				float alpha = powf(1.0f - (distance / maxDistance), e);
				alpha = alpha > 1.0f ? 1.0f : alpha;
				imageData[currentPixelOffset + 3] = (unsigned char)(alpha*255.0f);
			}
		}
		return imageData;
	}
	AliceAny GenRGBImage(int width, int height, AliceUInt8 r, AliceUInt8 g, AliceUInt8 b) {
		unsigned char *image_data = new unsigned char[width*height * 4];
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int currentPixelOffset = (x + y * width) * 4;
				image_data[currentPixelOffset] = r;
				image_data[currentPixelOffset + 1] = g;
				image_data[currentPixelOffset + 2] = b;
				image_data[currentPixelOffset + 3] = 255;
			}
		}
		return image_data;
	}
	bool SaveFreeTypeFontToPNG32(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height) {
		FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);
		BYTE*bits = FreeImage_GetBits(bitmap);
		AlphaToRGBA(255, imageData,width,height,bits);
		bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, filePath, PNG_DEFAULT);
		FreeImage_Unload(bitmap);
		return bSuccess;
	}
	bool SaveRGBAToPNG32(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height){
		FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);
		BYTE*bits = FreeImage_GetBits(bitmap);
		AliceUInt8*pixel = imageData;
		int dataLen = width*height;
		for (AliceSInt32 i = 0; i < dataLen; ++i){
			*bits++ = *pixel++;     //R
			*bits++ = *pixel++;     //G
			*bits++ = *pixel++;     //B
			*bits++ = *pixel++;     //A
		}
		bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, filePath, PNG_DEFAULT);
		FreeImage_Unload(bitmap);
		return bSuccess;
	}
	bool SaveRGBToJPEG24(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height){
		FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24, 8, 8, 8);
		BYTE*bits = FreeImage_GetBits(bitmap);
		AliceUInt8*pixel = imageData;
		int dataLen = width * height;
		for (AliceSInt32 i = 0; i < dataLen; ++i){
			*bits++ = *pixel++;     //R
			*bits++ = *pixel++;     //G
			*bits++ = *pixel++;     //B
		}
		bool bSuccess = FreeImage_Save(FIF_JPEG, bitmap, filePath, JPEG_DEFAULT);
		FreeImage_Unload(bitmap);
		return bSuccess;
	}
	bool SaveRToJPEG8(const char*filePath, AliceUInt8*imageData, AliceSInt32 width, AliceSInt32 height){
		FIBITMAP* bitmap = FreeImage_Allocate(width, height, 8, 8, 0, 0);
		BYTE*bits = FreeImage_GetBits(bitmap);
		AliceUInt8*pixel = imageData;
		int dataLen = width * height;
		for (AliceSInt32 i = 0; i < dataLen; ++i){
			*bits++ = *pixel++;     //R
		}
		bool bSuccess = FreeImage_Save(FIF_JPEG, bitmap, filePath, JPEG_DEFAULT);
		FreeImage_Unload(bitmap);
		return bSuccess;
	}
	void BGRAToRGBA(unsigned char * pixel, int width, int height) {
		int pixel_count = width * height * 4;
		for (int i = 0; i < pixel_count; i += 4) {
			unsigned char temp = pixel[i];
			pixel[i] = pixel[i + 2];
			pixel[i + 2] = temp;
		}
	}
	void BGRToRGB(unsigned char * pixel, int width, int height) {
		int pixel_count = width * height * 3;
		for (int i = 0; i < pixel_count; i += 3) {
			unsigned char temp = pixel[i];
			pixel[i] = pixel[i + 2];
			pixel[i + 2] = temp;
		}
	}
	void AlphaToRGBA(unsigned char luminance, unsigned char * pixel, int width, int height,unsigned char *out) {
		int pixel_count = width * height * 4;
		int j = 0;
		for (int i = 0; i < pixel_count; i+=4) {
			out[i] = luminance;
			out[i+1] = luminance;
			out[i + 2] = luminance;
			out[i + 3] = pixel[j++];
		}
	}
	bool GetImageInfo(unsigned char * const data, int dataLen, int&width, int&height, int&bpp) {
		FIMEMORY *m = FreeImage_OpenMemory(data, dataLen);
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileTypeFromMemory(m);
		if (fif == FIF_UNKNOWN) {
			FreeImage_CloseMemory(m);
			errorC("GetImageInfo cannot detect image type imagedata len %d", dataLen);
			return false;
		}
		FIBITMAP*srcImage = NULL;
		if (FreeImage_FIFSupportsReading(fif)) {
			srcImage = FreeImage_LoadFromMemory(fif, m);
		}
		else {
			FreeImage_CloseMemory(m);
			errorC("GetImageInfo fail,donot support image format of %d", fif);
			return false;
		}
		if (!srcImage) {
			FreeImage_CloseMemory(m);
			errorC("GetImageInfo fail cannot load image from memory buffer");
			return false;
		}
		FREE_IMAGE_TYPE srcType = FreeImage_GetImageType(srcImage);
		AliceUInt8*bits = FreeImage_GetBits(srcImage);
		int nWidth = FreeImage_GetWidth(srcImage);
		int nHeight = FreeImage_GetHeight(srcImage);;
		if ((bits == 0) || (nWidth == 0) || (nHeight == 0))
			return false;
		width = nWidth;
		height = nHeight;
		bpp = FreeImage_GetBPP(srcImage);
		FreeImage_Unload(srcImage);
		FreeImage_CloseMemory(m);
		return true;
	}
	int RawImage2Uncompressed32BITRGBA(unsigned char * const data, int dataLen, Alice::Data&outPixel,int&width, int&height) {
		width = 0;
		height = 0;
		FIMEMORY *m = FreeImage_OpenMemory(data, dataLen);
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileTypeFromMemory(m);
		if (fif == FIF_UNKNOWN) {
			FreeImage_CloseMemory(m);
			errorC("create texture with default method cannot detect image type imagedata len %d", dataLen);
			return 0;
		}
		FIBITMAP*srcImage = NULL;
		if (FreeImage_FIFSupportsReading(fif)) {
			srcImage = FreeImage_LoadFromMemory(fif, m);
		} else {
			FreeImage_CloseMemory(m);
			errorC("create texture with default method fail,donot support image format of %d", fif);
			return 0;
		}
		if (!srcImage) {
			FreeImage_CloseMemory(m);
			errorC("create texture with default method fail cannot load image from memory buffer");
			return 0;
		}
		FREE_IMAGE_TYPE srcType = FreeImage_GetImageType(srcImage);
		AliceUInt8*bits = FreeImage_GetBits(srcImage);
		int nWidth = FreeImage_GetWidth(srcImage);
		int nHeight = FreeImage_GetHeight(srcImage);

		AliceUInt32 bpp = FreeImage_GetBPP(srcImage);
		AliceUInt32 pitch = FreeImage_GetPitch(srcImage);
		if ((bits == 0) || (nWidth == 0) || (nHeight == 0))
			return 0;
		if (nWidth % 4 != 0) {
			nWidth = nWidth - (nWidth%4);
		}
		if (nHeight % 4 != 0) {
			nHeight = nHeight - (nHeight%4);
		}
		if (srcType == FIT_BITMAP) {
			if (bpp == 32) {
				int data_size = nWidth * nHeight * 4;
				outPixel.SetBufferSize(data_size);
				outPixel.mDataLen = data_size;
				BYTE*dstBits = outPixel.mData;
				for (int y = 0; y < nHeight; ++y) {
					BYTE*pixel = bits;
					BYTE*dstPixel = dstBits;
					for (int x = 0; x < nWidth; ++x) {
						*dstPixel++ = pixel[FI_RGBA_RED];
						*dstPixel++ = pixel[FI_RGBA_GREEN];
						*dstPixel++ = pixel[FI_RGBA_BLUE];
						*dstPixel++ = pixel[FI_RGBA_ALPHA];
						pixel += 4;
					}
					bits += pitch;
					dstBits += nWidth * 4;
				}
			}
			else if (bpp == 24) {
				int pitchPerRow = nWidth * 3;
				int data_size = nWidth * nHeight * 3;
				outPixel.SetBufferSize(data_size);
				outPixel.mDataLen = data_size;
				BYTE*dstBits = outPixel.mData;
				for (int y = 0; y < nHeight; ++y) {
					BYTE*pixel = bits;
					BYTE*dstPixel = dstBits;
					for (int x = 0; x < nWidth; ++x) {
						*dstPixel++ = pixel[FI_RGBA_RED];
						*dstPixel++ = pixel[FI_RGBA_GREEN];
						*dstPixel++ = pixel[FI_RGBA_BLUE];
						pixel += 3;
					}
					bits += pitch;
					dstBits += pitchPerRow;
				}
			}
			else if (bpp == 8) {
				int data_size = nWidth * nHeight ;
				outPixel.SetBufferSize(data_size);
				outPixel.mDataLen = data_size;
				BYTE*dstBits = outPixel.mData;
				for (int y = 0; y < nHeight; y++) {
					BYTE*pixel = bits;
					BYTE*dstPixel = dstBits;
					for (int x = 0; x < nWidth; x++) {
						*dstPixel++ = *pixel++;
					}
					bits += pitch;
					dstBits += nWidth;
				}
			}
			else {
				errorC("file bpp is not supported,must be 8¡¢24¡¢32");
				FreeImage_Unload(srcImage);
				FreeImage_CloseMemory(m);
				return 0;
			}
		}
		else {
			errorC("unsupport image internal type %d,only support standard image internal type", srcType);
			FreeImage_Unload(srcImage);
			FreeImage_CloseMemory(m);
			return 0;
		}
		width = nWidth;
		height = nHeight;
		FreeImage_Unload(srcImage);
		FreeImage_CloseMemory(m);
		return bpp;
	}
}
