#include "CharInfo.h"

namespace Alice
{
	CharInfo::CharInfo():mAtlasRectangle(nullptr),mAtlas(nullptr),mCharStyle(kCharStyleNormal),mCharCode(0)
	{

	}
	CharInfo::~CharInfo() {
		if (mAtlasRectangle!=nullptr){
			delete mAtlasRectangle;
		}
	}
}