#pragma once
#include "Runtime/2D/Label.h"

namespace Alice
{
	class ExportedLabel :public Label
	{
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(GetRect);
		DEFINE_LUA_API(GetLocalRect);
		DEFINE_LUA_API(GetPixelLen);
		DEFINE_LUA_API(GetHeight);
		DEFINE_LUA_API(GetAligning);
		DEFINE_LUA_API(SetAligning);
		DEFINE_LUA_API(SetCharSize);
		DEFINE_LUA_API(GetStyle);
		DEFINE_LUA_API(SetStyle);
		DEFINE_LUA_API(SetBold);
		DEFINE_LUA_API(GetBold);
		DEFINE_LUA_API(GetColor);
		DEFINE_LUA_API(SetColor);
		DEFINE_LUA_API(GetText);
		DEFINE_LUA_API(SetText);
		DEFINE_LUA_API(AppendText);
		DEFINE_LUA_API(TrimEnd);
		DEFINE_LUA_API(DeleteAt);//0~
		DEFINE_LUA_API(InsertTextAt);//0~
		DEFINE_LUA_API(GetCharCount);
		DEFINE_LUA_API(GetCharWidthInPixel);
		DEFINE_LUA_API(GetLabelHeightInPixel);
		DEFINE_LUA_API(GetCharAt);
		DEFINE_LUA_API(GetFont);
		DEFINE_LUA_API(SetFont);
		DEFINE_LUA_API(SetDynamicFont);
		DEFINE_LUA_API(GetOutlineColor);
		DEFINE_LUA_API(SetOutlineColor);
		DEFINE_LUA_API(GetOutlineSize);
		DEFINE_LUA_API(SetOutlineSize);
		DEFINE_LUA_API(GetShadow);
		DEFINE_LUA_API(SetShadow);
		DEFINE_LUA_API(GetShadowColor);
		DEFINE_LUA_API(SetShadowColor);
		DEFINE_LUA_API(SetMaterial);
		DEFINE_LUA_API(GetMaterial);
		DEFINE_LUA_API(UseStencilMask);
		DEFINE_LUA_API(UseFont);
		DEFINE_LUA_API(PointSizeToPixelSize);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedLabel, Label)
	};
}
