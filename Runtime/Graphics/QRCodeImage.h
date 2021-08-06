#pragma once
namespace Alice {
	class QRCodeImage {
	public:
		static int String2Image(const char *str,int len,unsigned char * imageData,int size);
	};
}