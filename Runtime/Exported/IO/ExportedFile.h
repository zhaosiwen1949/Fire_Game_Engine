#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
namespace Alice{
	class ExportedFile{
	public:
		DEFINE_LUA_API(Exist);
		DEFINE_LUA_API(Delete);
		static int Export(lua_State*L);
	};
}
