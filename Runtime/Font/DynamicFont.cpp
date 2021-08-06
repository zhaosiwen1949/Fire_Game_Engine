#include "DynamicFont.h"
#include "Runtime/RuntimePrefix.h"
#include FT_BBOX_H
#include "Runtime/String/StringUtils.h"
#include "Runtime/Debugger/Debugger.h"
#include <unordered_map>
#include "Runtime/IO/FileSystem.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Math/Rect.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "Runtime/IO/FontResource.h"
#if ALICE_PLATFORM_WIN
#pragma comment(lib,"freetype248_D.lib")
#endif

#define INIT_CHAR_ID(size,style,outline_size,shadow_x,shadow_y,charCode) \
AliceUInt64 charID; \
do{ \
char*pStyle = (char*)&charID; \
*(pStyle++) = size+style; \
*(pStyle++) = outline_size; \
*(pStyle++) = shadow_x; \
*(pStyle++) = shadow_y; \
*((unsigned int*)pStyle) = charCode;\
}while(0)

namespace Alice{
	DynamicFont* DynamicFont::mDefaultFont;
	static int sDynamicFontTextureSize = 0;
	DynamicFont::DynamicFont():mFace(nullptr) {
	}
	DynamicFont::~DynamicFont() {
		for (std::unordered_map<AliceUInt64, CharInfo*>::iterator iter = mCachedChars.begin(); iter != mCachedChars.end(); ++iter) {
			CharInfo*char_info = iter->second;
			delete char_info;
		}
		mCachedChars.clear();
		for (std::list<DynamicTextureAtlas*>::iterator iter = mAtlases.begin(); iter != mAtlases.end(); iter++) {
			 DynamicTextureAtlas*texture=*iter;
			 delete texture;
		}
		mAtlases.clear();
	}
	void DynamicFont::InitFTLib(){
		FontFace::InitFreeType();
		Alice::FontResource*fontData = Alice::Resource::LoadResource<Alice::FontResource>("builtin/Font/Arial.ttf");
		if (fontData != nullptr) {
			FontFace*face = new (kMemFontId)FontFace;
			if (face->Init(fontData)) {
				face->mFontData = fontData;
				DynamicFont*fft = new (kMemFontId)DynamicFont;
				fft->mFace = face;
				mCachedFonts.insert(std::pair<std::string, DynamicFont*>("Arial", fft));
			}
			else {
				delete fontData;
				delete face;
			}
		}
	}
	std::unordered_map<std::string, DynamicFont*> DynamicFont::mCachedFonts;
	DynamicFont* DynamicFont::GetFont(const char*fontName){
		std::unordered_map<std::string, DynamicFont*>::iterator iter = mCachedFonts.find(fontName);
		if (iter == mCachedFonts.end()){
			FontResource*font = Resource::LoadResource<FontResource>(fontName);
			if (font != nullptr){
				FontFace*face = new (kMemFontId)FontFace;
				if (face->Init(font)){
					face->mFontData = font;
					DynamicFont*fft = new (kMemFontId)DynamicFont;
					fft->mFace = face;
					mCachedFonts.insert(std::pair<std::string, DynamicFont*>(fontName,fft));
					return fft;
				} else {
					delete font;
					delete face;
				}
			}
			return nullptr;
		}
		return iter->second;
	}
	CharInfo*DynamicFont::GetCharInfoWithCharID(AliceUInt64 char_id) {
		std::unordered_map<AliceUInt64, CharInfo*>::iterator iter = mCachedChars.find(char_id);
		if (iter!=mCachedChars.end()){
			return iter->second;
		}
		return nullptr;
	}
	void DynamicFont::CheckChar(AliceUInt32&unicodeChar) {
		if (unicodeChar == 9) {
			unicodeChar = 32;//tab -> space
		}
	}
	bool DynamicFont::UpdateFTMatrix(CharStyle style) {
		bool bold = false;
		float lean = 0.2f;
		if (style == kCharStyleNormal) {
			lean = 0.0f;
		}
		else if (style == kCharStyleBold) {
			lean = 0.0f;
			bold = true;
		}
		else if (style == kCharStyleItalicBold) {
			bold = true;
		}
		FT_Matrix matrix;
		matrix.xx = 0x10000L;
		matrix.xy = lean * 0x10000L;
		matrix.yx = 0;
		matrix.yy = 0x10000L;
		FT_Set_Transform(mFace->mFace, &matrix, 0);
		return bold;
	}
	CharInfo*DynamicFont::NewChar(AliceUInt64 charID, AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, AliceSInt8 outlineSize, AliceSInt8 shadow_x, AliceSInt8 shadow_y) {
		bool bold = UpdateFTMatrix(style);
		Alice::Data imageData;
		CharInfo*tempChar = nullptr;
		FT_Glyph glyph = mFace->GetGlyph(unicodeChar, size);
		if (glyph != nullptr) {
			mFace->SetBoldSetting(glyph,bold);
			FT_Bitmap *font_bmp = mFace->OutlineGlyphToBitmap(glyph);
			FT_Bitmap *stroker_bmp = nullptr;
			if (outlineSize > 0) {
				stroker_bmp = mFace->GetStrokerBitMap(glyph, outlineSize);
			}
			tempChar = mFace->RenderingCurrentChar(unicodeChar,font_bmp, stroker_bmp,shadow_x,shadow_y,imageData);
			FT_Done_Glyph(glyph);
			if (font_bmp!=nullptr){
				delete[] font_bmp->buffer;
				delete font_bmp;
			}
			if (stroker_bmp!=nullptr){
				delete[] stroker_bmp->buffer;
				delete stroker_bmp;
			}
		}
		CachingChar(tempChar, charID,imageData.mData);
		return tempChar;
	}

	void DynamicFont::CachingChar(CharInfo *tempChar, AliceUInt64 charID, AliceUInt8*charData){
		DynamicTextureAtlas*atlas = nullptr;
		TextureRectangleNode*insertToNode = nullptr;
		int charWidth = tempChar->mWidth;
		int charHeight = tempChar->mHeight;
		if (!mAtlases.empty()){
			for (std::list<DynamicTextureAtlas*>::iterator iter = mAtlases.begin(); iter != mAtlases.end(); iter++){
				insertToNode = (*iter)->AddTextureTile(charID, charWidth, charHeight, charData);
				if (insertToNode != nullptr){
					atlas = *iter;
					break;
				}
			}
		}
		if (atlas == nullptr){
			if (sDynamicFontTextureSize == 0) {
				sDynamicFontTextureSize = 1024;// DeviceInfo::MaxTextureSize() > 4096 ? 4096 : DeviceInfo::MaxTextureSize();
			}
			atlas = new (kMemFontId)DynamicTextureAtlas(sDynamicFontTextureSize, sDynamicFontTextureSize);
			insertToNode = atlas->AddTextureTile(charID, charWidth, charHeight, charData);
			mAtlases.push_back(atlas);
		}
		tempChar->mAtlasRectangle = insertToNode;
		tempChar->mAtlas = atlas;
		mCachedChars.insert(std::pair<AliceUInt64, CharInfo*>(charID, tempChar));
	}
	bool DynamicFont::CreateChar(AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, CharInfo**charInfo, AliceSInt8 outlineSize /* = 0 */, AliceSInt8 shadow_x /* = 0 */, AliceSInt8 shadow_y /* = 0 */) {
		CheckChar(unicodeChar);
		INIT_CHAR_ID(size, style, outlineSize, shadow_x, shadow_y, unicodeChar);
		CharInfo*char_info = GetCharInfoWithCharID(charID);
		if (char_info != nullptr) {
			*charInfo = char_info;
			return true;
		}
		*charInfo = NewChar(charID, unicodeChar, style, size, outlineSize, shadow_x, shadow_y);
		return true;
	}
	bool DynamicFont::CreateDisplayChar(const char*fontName, AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, CharInfo**charInfo, AliceSInt8 outlineSize /* = 0 */, AliceSInt8 shadow_x /* = 0 */, AliceSInt8 shadow_y /* = 0 */){
		DynamicFont* ttfInfo = GetFont(fontName);
		return ttfInfo->CreateChar(unicodeChar,style,size,charInfo,outlineSize,shadow_x,shadow_y);
	}
}
