#pragma once
#include "FontFace.h"
#include "Runtime/IO/ResourceManager.h"
#include <string>
#include <unordered_map>
#include <list>
#include "Runtime/Serializer/TextureAtlas.serializer.h"
#include "Runtime/IO/FontResource.h"
#include "CharInfo.h"

namespace Alice{
class DynamicFont : public Object{
public:
	DynamicFont();
	~DynamicFont();
	SmartPtr<FontFace> mFace;
	//key is : [size+charstyle](8bit)+bold_strength(4bit)+outlinesize(4bit)+shadow_x(8bit)+shadow_y(8bit)+  unicode(32bit)
	std::unordered_map<AliceUInt64, CharInfo*> mCachedChars;
	std::list<DynamicTextureAtlas*> mAtlases;
public:
	CharInfo * GetCharInfoWithCharID(AliceUInt64 char_id);
	bool UpdateFTMatrix(CharStyle style);
	CharInfo * NewChar(AliceUInt64 charID,AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, AliceSInt8 outlineSize, AliceSInt8 shadow_x, AliceSInt8 shadow_y);
	bool CreateChar(AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, CharInfo**charInfo, AliceSInt8 outlineSize = 0, AliceSInt8 shadow_x = 0, AliceSInt8 shadow_y = 0);
	void CachingChar(CharInfo *tempChar, AliceUInt64 charID, AliceUInt8*charData);
public:
	static DynamicFont* mDefaultFont;
	static std::unordered_map<std::string, DynamicFont*> mCachedFonts;
	static bool CreateDisplayChar(const char*fontName, AliceUInt32 unicodeChar, CharStyle style, AliceSInt8 size, CharInfo**charInfo, AliceSInt8 outlineSize = 0, AliceSInt8 shadow_x = 0, AliceSInt8 shadow_y = 0);
	static void InitFTLib();
	static DynamicFont* GetFont(const char*fontName);
	static void CheckChar(AliceUInt32&unicodeChar);
public:
	DECLEAR_ALICE_CLASS(DynamicFont)
};
}
