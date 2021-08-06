#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/String/StringUtils.h"
namespace Alice{
	class ExportedString {
	public:
		DEFINE_LUA_API(UnicodeToASCII);
		DEFINE_LUA_API(UnicodeToUTF8);
		DEFINE_LUA_API(UTF8ToUnicode);
		DEFINE_LUA_API(ASCIIToUnicode);
		DEFINE_LUA_API(UTF8ToASCII);
		DEFINE_LUA_API(ASCIIToUTF8);
		static int Export(lua_State*L);
	};
}
