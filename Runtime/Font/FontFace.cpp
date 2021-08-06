#include "FontFace.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/Math/Rect.h"
#include "Runtime/Graphics/ImageUtils.h"
namespace Alice {
#define ALICE_TO_26_6_FORMAT(x) ((x)<<6)
#define ALICE_FROM_26_6_FORMAT(x) ((x)>>6)
#define ALICE_TO_OUTLINE_GLYPH(glyph) (reinterpret_cast<FT_OutlineGlyph>(glyph))
	static void RenderOutlineToBitmap(const int y,
		const int count,
		const FT_Span * const spans,
		void * const user) {

	}
	static FT_Library sFTlib;
	//全平台都使用96的dpi去生成文字图片
	static int sDeviceDPIX = 96, sDeviceDPIY = 96;
	FontFace::FontFace() : mFace(nullptr), mFontData(nullptr) {
	}
	FontFace::~FontFace() {
		if (mFace != nullptr) {
			FT_Done_Face(mFace);
		}
	}
	bool FontFace::Init(FontResource*fontData) {
		if (FT_New_Memory_Face(sFTlib, (AliceUInt8*)((Serializer::RuntimeAsset*)fontData->mAssetBundle)->rawdata().c_str(), ((Serializer::RuntimeAsset*)fontData->mAssetBundle)->rawdata().length(), 0, &mFace) != 0) {
			Error("FontFace::Init %s fail", fontData->mName.mText);
			return false;
		}
		if (FT_IS_SCALABLE(mFace)) {
			Debug("FontFace::Init Font Face %s is scalable", fontData->mName.mText);
		}
		else {
			Debug("FontFace::Init Font Face %s is not scalable", fontData->mName.mText);
		}
		Debug("face count %d", mFace->num_faces);
		Debug("face 0 glyph count %d", mFace->num_glyphs);
		Debug("face 0 fixed bitmap count %d", mFace->num_fixed_sizes);
		Debug("face 0 charmap count %d", mFace->num_charmaps);
		return true;
	}
	bool FontFace::InitWithFontFileData(unsigned char*fontData, int len) {
		if (FT_New_Memory_Face(sFTlib, fontData, len, 0, &mFace) != 0) {
			Error("FontFace::InitWithFontFileData fail");
			return false;
		}
		if (FT_IS_SCALABLE(mFace)) {
			Debug("FontFace::InitWithFontFileData Font Face is scalable");
		}
		else {
			Debug("FontFace::InitWithFontFileData Font Face is not scalable");
		}
		return true;
	}
	void FontFace::InitFreeType() {
		int err = FT_Init_FreeType(&sFTlib);
		int major, minor, patch;
		FT_Library_Version(sFTlib, &major, &minor, &patch);
	}
	int FontFace::PixelSize2PointSize(int pixelSize) {
		int point_size = FloorfToInt((float(pixelSize) * 72.0f) / float(sDeviceDPIY));
		return point_size;
	}
	float FontFace::PointSize2PixelSize(int point_size) {
		float pixel_size = (float(point_size) * float(sDeviceDPIY)) / 72.0f;
		return pixel_size;
	}
	float FontFace::GetLineHeight(int pt) {
		int dpix = 96;//FloorfToInt(DeviceInfo::GetDPIX());
		int dpiy = 96;//FloorfToInt(DeviceInfo::GetDPIY());
		if (FT_Set_Char_Size(mFace, ALICE_TO_26_6_FORMAT(pt), ALICE_TO_26_6_FORMAT(pt), dpix, dpiy) == 0) {
			return float(ALICE_FROM_26_6_FORMAT(mFace->height))*mFace->size->metrics.y_scale / 65536.0f;
		}
		return 0;
	}
	FT_Glyph FontFace::GetGlyph(AliceUInt32 charCode, int point_size) {
		int dpix = 96;//FloorfToInt(DeviceInfo::GetDPIX());
		int dpiy = 96;//FloorfToInt(DeviceInfo::GetDPIY());
		if (FT_Set_Char_Size(mFace, ALICE_TO_26_6_FORMAT(point_size), ALICE_TO_26_6_FORMAT(point_size), dpix, dpiy) == 0) {
			if (0 == FT_Load_Char(mFace, charCode, FT_LOAD_DEFAULT)) {
				if (mFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
					return nullptr;
				}
				FT_Glyph glyph;
				FT_Get_Glyph(mFace->glyph, &glyph);
				return glyph;
			}
		}
		return nullptr;
	}
	void FontFace::SetBoldSetting(FT_Glyph glyph, bool bold) {
		int strength = 0;
		if (bold) {
			strength = 1;//4
		}
		FT_OutlineGlyph outline_glyph = ALICE_TO_OUTLINE_GLYPH(glyph);
		if (0 != FT_Outline_Embolden(&outline_glyph->outline, ALICE_TO_26_6_FORMAT(strength))) {
			Error("FontFace::SetBoldSetting failed %d", strength);
		}
	}
	FT_Bitmap*FontFace::OutlineGlyphToBitmap(FT_Glyph glyph) {
		if (glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
			FT_OutlineGlyph outline_glyph = ALICE_TO_OUTLINE_GLYPH(glyph);
			FT_Outline *outline = &outline_glyph->outline;
			FT_BBox  bbox;
			FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_GRIDFIT, &bbox);
			int charWidth = ALICE_FROM_26_6_FORMAT(bbox.xMax - bbox.xMin);
			int charHeight = ALICE_FROM_26_6_FORMAT(bbox.yMax - bbox.yMin);
			FT_Bitmap *bmp = new FT_Bitmap;
			bmp->buffer = new unsigned char[charWidth*charHeight];
			memset(bmp->buffer, 0, charHeight*charWidth);
			bmp->width = charWidth;
			bmp->rows = charHeight;
			bmp->pitch = charWidth;
			bmp->pixel_mode = FT_PIXEL_MODE_GRAY;
			bmp->num_grays = 256;
			FT_Outline_Translate(outline, -bbox.xMin, -bbox.yMin);
			FT_Raster_Params params;
			memset(&params, 0, sizeof(params));
			params.flags = FT_RASTER_FLAG_AA;
			params.gray_spans = NULL;
			params.user = NULL;
			params.target = bmp;
			FT_Outline_Render(sFTlib, outline, &params);
			return bmp;
		}
		return nullptr;
	}
	FT_Bitmap * FontFace::GetStrokerBitMap(FT_Glyph glyph, int outline_size) {
		FT_Stroker stroker;
		FT_Stroker_New(sFTlib, &stroker);
		FT_Stroker_Set(stroker, (int)(ALICE_TO_26_6_FORMAT(outline_size)), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
		FT_Glyph_StrokeBorder(&glyph, stroker, 0, 0);//这个操作会改变glyph变量指向的内容
		FT_Bitmap *ret = OutlineGlyphToBitmap(glyph);
		FT_Stroker_Done(stroker);
		FT_Done_Glyph(glyph);
		return ret;
	}
	CharInfo*FontFace::RenderingCurrentChar(AliceUInt32 charCode, FT_Bitmap*font, FT_Bitmap*stroker, int shadow_x, int shadow_y, Alice::Data&imageData) {
		int charWidth = font->width;
		int charHeight = font->rows;
		int font_image_offset_from_origion_x = 0;
		int font_image_offset_from_origion_y = 0;
		int stroker_image_offset_from_origion_x = 0;
		int stroker_image_offset_from_origion_y = 0;
		int horizontal_offset = 0;
		int baseline_offset = 0;
		int font_end_x = charWidth;
		int font_end_y = charHeight;
		int stroker_end_x = 0;
		int stroker_end_y = 0;
		bool need_render_shadow = false;
		if (stroker != nullptr) {
			charWidth = stroker->width;
			charHeight = stroker->rows;
			font_image_offset_from_origion_x = (charWidth - font->width) / 2;
			font_end_x = font->width + font_image_offset_from_origion_x;
			font_image_offset_from_origion_y = (stroker->rows - font->rows) / 2;
			font_end_y = font->rows + font_image_offset_from_origion_y;
			horizontal_offset = font_image_offset_from_origion_y;
			stroker_end_x = stroker->width;
			stroker_end_y = stroker->rows;
		}
		if (shadow_x > 0) {
			charWidth += shadow_x;
			need_render_shadow = true;
		}
		else if (shadow_x < 0) {
			charWidth -= shadow_x;
			font_image_offset_from_origion_x -= shadow_x;
			stroker_image_offset_from_origion_x -= shadow_x;
			font_end_x -= shadow_x;
			stroker_end_x -= shadow_x;
			need_render_shadow = true;
		}
		if (shadow_y > 0) {
			charHeight += shadow_y;
			horizontal_offset += shadow_y;
			need_render_shadow = true;
		}
		else if (shadow_y < 0) {
			charHeight -= shadow_y;
			font_image_offset_from_origion_y -= shadow_y;
			stroker_image_offset_from_origion_y -= shadow_y;
			font_end_y -= shadow_y;
			stroker_end_y -= shadow_y;
			need_render_shadow = true;
		}
		baseline_offset = horizontal_offset;
		int imgSize = charWidth * charHeight * 4;//RGBA
		imageData.SetBufferSize(imgSize);
		AliceUInt8*imgData = imageData.mData;
		memset(imgData, 0, imgSize);
		CharInfo *ci = new (kMemFontId)CharInfo;
		ci->mCharCode = charCode;
		if (stroker != nullptr) {
			ci->mAdvanceX = float(ALICE_FROM_26_6_FORMAT(mFace->glyph->advance.x)) + (stroker->width - font->width);
		}
		else {
			ci->mAdvanceX = float(ALICE_FROM_26_6_FORMAT(mFace->glyph->advance.x));
		}
		ci->mHoriBearingX = float(ALICE_FROM_26_6_FORMAT(mFace->glyph->metrics.horiBearingX)) + font_image_offset_from_origion_x;
		ci->mHoriBearingY = float(ALICE_FROM_26_6_FORMAT(mFace->glyph->metrics.horiBearingY)) + horizontal_offset;
		ci->mWidth = charWidth;
		ci->mHeight = charHeight;
		ci->mBaselineOffset = float(ALICE_FROM_26_6_FORMAT(mFace->size->metrics.ascender)) + baseline_offset + float(ALICE_FROM_26_6_FORMAT(mFace->size->metrics.descender));
		if (charCode == 32) {//space
			return ci;
		}
		for (int y = 0; y < charHeight; ++y) {
			for (int x = 0; x < charWidth; ++x) {
				int current_offset = y * charWidth + x;
				bool bHaveShadow = false;
				if (stroker != nullptr) {
					if (x >= stroker_image_offset_from_origion_x && x <= stroker_end_x &&
						y >= stroker_image_offset_from_origion_y && y <= stroker_end_y) {
						int stroker_x = x - stroker_image_offset_from_origion_x;
						int stroker_y = y - stroker_image_offset_from_origion_y;
						if (stroker_x < stroker->width&&stroker_y < stroker->rows) {
							int curret_font_offset = stroker->width*(stroker->rows - stroker_y - 1) + stroker_x;
							imgData[current_offset * 4] = stroker->buffer[curret_font_offset];
						}
					}
				}
				if (x >= font_image_offset_from_origion_x && x <= font_end_x &&
					y >= font_image_offset_from_origion_y && y <= font_end_y) {
					int font_x = x - font_image_offset_from_origion_x;
					int font_y = y - font_image_offset_from_origion_y;
					if (font_x < font->width&&font_y < font->rows) {
						int curret_font_offset = font->width*(font->rows - font_y - 1) + font_x;
						imgData[current_offset * 4 + 3] = font->buffer[curret_font_offset];
					}
				}
				if (need_render_shadow) {
					int fixed_shadow_x = x + shadow_x;
					int fixed_shadow_y = y + shadow_y;
					AliceUInt8 outline_value = imgData[current_offset * 4];
					AliceUInt8 font_value = imgData[current_offset * 4 + 3];
					if (outline_value > 0 || font_value > 0) {
						int shadow_offset = charWidth * fixed_shadow_y + fixed_shadow_x;
						if (outline_value > 0) {
							imgData[shadow_offset * 4 + 1] = outline_value;
						}
						else if (font_value > 0) {
							imgData[shadow_offset * 4 + 1] = font_value;
						}
					}
				}
			}
		}
		return ci;
	}
}