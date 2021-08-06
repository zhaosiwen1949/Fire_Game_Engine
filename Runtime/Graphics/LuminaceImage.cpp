#include "LuminaceImage.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/IO/AliceData.h"
#include "Runtime/Graphics/ImageUtils.h"
#include <zxing/common/IllegalArgumentException.h>
namespace Alice {
	LuminaceImage::LuminaceImage(int width, int height):LuminanceSource(width,height) {
	}
	zxing::Ref<zxing::LuminanceSource> LuminaceImage::create(unsigned char *imagedata, int len) {
		Alice::Data data;
		GLenum cpuFormat = GL_RGBA;
		GLenum gpuFormat = GL_RGBA;
		int nWidth = 0, nHeight = 0;
		int bpp = Alice::RawImage2Uncompressed32BITRGBA(imagedata, len, data, nWidth, nHeight);
		LuminaceImage*image = nullptr;
		if (bpp == 24) {
			image = new LuminaceImage(nWidth, nHeight);
			image->mImageData.SetBufferSize(nWidth*nHeight);
			int width = nWidth;
			int height = nHeight;
			for (int y = 0; y < height; ++y) {
				int yoffset = y * width;
				int yoffset2 = (height - y - 1) * width * 3;
				unsigned char *p = image->mImageData.mData + yoffset;
				for (int x = 0; x < width; ++x, ++p) {
					unsigned char r = data.mData[yoffset2 + x * 3];
					unsigned char g = data.mData[yoffset2 + x * 3 + 1];
					unsigned char b = data.mData[yoffset2 + x * 3 + 2];
					*p = (unsigned char)((float)(r + g + b) / 3.0f);
				}
			}
		}
		else if (bpp == 32) {
			int width = nWidth;
			int height = nHeight;
			image = new LuminaceImage(nWidth, nHeight);
			image->mImageData.SetBufferSize(nWidth*nHeight);
			for (int y = 0; y < height; ++y) {
				int yoffset = y * width;
				unsigned char *p = image->mImageData.mData + yoffset;
				int yoffset2 = (height - y - 1) * width * 4;
				for (int x = 0; x < width; ++x, ++p) {
					unsigned char r = data.mData[yoffset2 + x * 4];
					unsigned char g = data.mData[yoffset2 + x * 4 + 1];
					unsigned char b = data.mData[yoffset2 + x * 4 + 2];
					*p = (unsigned char)((float)(r + g + b) / 3.0f);
				}
			}
		}
		else if (bpp == 8) {
			int width = nWidth;
			int height = nHeight;
			image = new LuminaceImage(nWidth, nHeight);
			image->mImageData.SetBufferSize(nWidth*nHeight);
			for (int y = 0; y < height; ++y) {
				int yoffset = y * width;
				int yoffset2 = (height - y - 1) * width;
				unsigned char *p = image->mImageData.mData + yoffset;
				for (int x = 0; x < width; ++x, ++p) {
					unsigned char r = data.mData[yoffset2 + x * 4];
					*p = r;
				}
			}
		}
		else {
			return zxing::Ref<LuminanceSource>();
		}
		return zxing::Ref<LuminanceSource>(image);
	}
	zxing::ArrayRef<char> LuminaceImage::getRow(int y, zxing::ArrayRef<char> row) const {
		// Get width
		int width = getWidth();
		if (!row) {
			// Create row
			row = zxing::ArrayRef<char>(width);
		}
		// Get pointer to row
		const char *p = (char*)mImageData.mData+ y*width;
		for (int x = 0; x < width; ++x, ++p) {
			// Set row at index x
			row[x] = *p;
		}
		return row;
	}
	zxing::ArrayRef<char> LuminaceImage::getMatrix() const {
		// Get width and height
		int width = getWidth();
		int height = getHeight();
		// Create matrix
		zxing::ArrayRef<char> matrix = zxing::ArrayRef<char>(width * height);
		for (int y = 0; y < height; ++y) {
			// Get pointer to row
			const char *p = (char*)mImageData.mData + y * width;
			// Calculate y offset
			int yoffset = y * width;
			for (int x = 0; x < width; ++x, ++p) {
				// Set row at index x with y offset
				matrix[yoffset + x] = *p;
			}
		}
		return matrix;
	}
}
