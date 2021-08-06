#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/LuaEngine/LuaEngine.h"

namespace Alice{
	class ExportedLuaEngine :public LuaEngine{
	public:
		static int sUserPrintAgent;
		static int sUserErrorAgent;
		static void ExecuteUserScript(lua_State*L, const char*scriptName, const char*code, int codeLen);
		DEFINE_LUA_API(UserPrint);//override print function
		DEFINE_LUA_API(SetUserPrintAgent);//override print function
		DEFINE_LUA_API(EvalUserScript);//override require function
	public:
		DEFINE_LUA_API(require);//override require function
		DEFINE_LUA_API(CoreVersion);
		DEFINE_LUA_API(ShowKeyboard);
		DEFINE_LUA_API(HideKeyboard);
		DEFINE_LUA_API(JumpTo);//download init resources->set "Assets" table to nil->init from inited resources
		DEFINE_LUA_API(Exit);
		DEFINE_LUA_API(IsExpVersion);
#ifdef ALICE_WIN_PLAYER
		DEFINE_LUA_API(CHDIR);
#endif
		DEFINE_LUA_API(GC);
		static int Export(lua_State*L);
	};
}
