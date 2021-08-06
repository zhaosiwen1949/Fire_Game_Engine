#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Font/DynamicFont.h"
namespace Alice{
	class ExportedDynamicFont : public DynamicFont {
	public:
		DEFINE_LUA_API(SetFont);
		DEFINE_LUA_API(SetFontData);
		DEFINE_LUA_API(GetGlobalHeight);
		DEFINE_LUA_API(Release);
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedDynamicFont, DynamicFont)
	};
}
