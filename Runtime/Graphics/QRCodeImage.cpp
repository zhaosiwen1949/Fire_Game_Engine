#include "QRCodeImage.h"
#include "Runtime/Plugins/libqrencode/qrencode.h"
#include "ImageUtils.h"
#include "Runtime/Debugger/log.h"
namespace Alice {
	int QRCodeImage::String2Image(const char *str, int len, unsigned char * image_data, int size) {
		QRcode*code = QRcode_encodeString(str, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
		int scale = size / code->width;
		int left_part = size % code->width;
		int margin_left = 0;
		int margin_top = 0;
		//int right_edge = size;
		if (left_part>0){
			int x = left_part / 2;
			//int y = left_part % 2;
			margin_left = x;
			margin_top = x;
		}
		unsigned char *src = code->data;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				int nIndex = (i * size+j) * 4;
				image_data[nIndex] = 255;
				image_data[nIndex + 1] = 255;
				image_data[nIndex + 2] = 255;
				image_data[nIndex + 3] = 255;
			}
		}
		for (int i = 0; i < code->width; ++i) {
			for (int j = 0; j < code->width; ++j) {
				if (*src & 1) {
					for (int x = 0; x < scale; ++x) {
						for (int y = 0; y < scale; ++y) {
							int nIndex = (((code->width-i-1)*scale + y + margin_top ) * size + j*scale + margin_left+x) * 4;
							image_data[nIndex] = 0;
							image_data[nIndex + 1] = 0;
							image_data[nIndex + 2] = 0;
							image_data[nIndex + 3] = 255;
						}
					}
				}
				++src;
			}
		}
		return 0;
	}
}
