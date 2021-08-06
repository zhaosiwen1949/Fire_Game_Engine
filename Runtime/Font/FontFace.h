#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/IO/FontResource.h"
#include "CharInfo.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
namespace Alice{
	class FontFace : public Object {
	public:
		FontFace();
		~FontFace();
		FT_Face mFace;
		SmartPtr<FontResource> mFontData;
	public:
		bool Init(FontResource*fontData);
		bool InitWithFontFileData(unsigned char*fontData, int len);
		FT_Glyph GetGlyph(AliceUInt32 charCode, int point_size);
		void SetBoldSetting(FT_Glyph glyph, bool bold);
		FT_Bitmap * GetStrokerBitMap(FT_Glyph glyph, int outline_size);
		FT_Bitmap * OutlineGlyphToBitmap(FT_Glyph glyph);
		CharInfo * RenderingCurrentChar(AliceUInt32 charCode, FT_Bitmap*font, FT_Bitmap*stroker, int shadow_x, int shadow_y, Alice::Data&imageData);
		float GetLineHeight(int pt);
	public:
		static void InitFreeType();
		static int PixelSize2PointSize(int pixelSize);
		static float PointSize2PixelSize(int point_size);
	};
}