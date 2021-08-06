#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Graphics/DynamicTextureAtlas.h"
#include "Runtime/Math/Rect.h"
namespace Alice{
	enum CharStyle{
		kCharStyleNormal = 0,
		kCharStyleBold = 1 << 0,
		kCharStyleItalic = 1 << 1,
		kCharStyleItalicBold = 1 << 2,
	};
	class CharInfo{
	public:
		AliceUInt32 mCharCode;
		AliceUInt32 mByteCount;
		float	mWidth;
		float	mHeight;
		float	mAdvanceX;
		float	mHoriBearingX;
		float	mHoriBearingY;
		float	mBaselineOffset;
		CharStyle mCharStyle;
		DynamicTextureAtlas*mAtlas;
		TextureRectangleNode*mAtlasRectangle;//image data : rect,texcoord,
		CharInfo();
		~CharInfo();
	};
}