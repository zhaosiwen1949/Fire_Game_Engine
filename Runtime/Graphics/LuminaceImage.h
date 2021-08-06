#pragma once
#include "Runtime/IO/AliceData.h"
#include <zxing/LuminanceSource.h>
namespace Alice {
	class LuminaceImage : public zxing::LuminanceSource {
	private:
		Data mImageData;
	public:
		LuminaceImage(int width,int height);
		static zxing::Ref<zxing::LuminanceSource> create(unsigned char *imagedata, int len);
		zxing::ArrayRef<char> getRow(int y, zxing::ArrayRef<char> row) const;
		zxing::ArrayRef<char> getMatrix() const;
	};
}
