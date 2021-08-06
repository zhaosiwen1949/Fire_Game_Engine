#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/RuntimePrefix.h"

#define Lua_GetFunction(foo) Alice::LuaEngine::PrepareMethod(foo)
#define Lua_PushNil(c) Alice::LuaEngine::PushNil(c)
#define Lua_PushUserData(userdata) Alice::LuaEngine::PushUserData(userdata)
#define Lua_NewTable() Alice::LuaEngine::NewTable()
#define Lua_PushInt(i) Alice::LuaEngine::PushInt(i)
#define Lua_PushBoolean(i) Alice::LuaEngine::PushBoolean(i)
#define Lua_PushNumber(n) Alice::LuaEngine::PushNumber(n)
#define Lua_PushString(str) Alice::LuaEngine::PushString(str)
#define Lua_PushLString(str,len) Alice::LuaEngine::PushLString(str,len)
#define Lua_Invoke(hint) Alice::LuaEngine::Invoke(hint)

namespace Alice{
	class LuaEngine{
	public:
		static lua_State*	s_GlobalStatePtr;
		static void Init();
		static lua_State* GetLuaState();
		static void RecycleLuaState(lua_State*L);
		static int CallLibMethod(lua_State*L);
		static void CleanPlugins();
		static lua_State* Store();
		static int Restore();
		static int mCurrentArgCount;
		static bool PrepareMethod(int foo);
		static void PushInt(int a);
		static void PushBoolean(bool a);
		static void PushNumber(double a);
		static void PushString(const char *str);
		static void PushLString(const char *str, int len);
		static void PushNil(int count = 1);
		static void PushUserData(int userdata);
		static void NewTable ();
		static void Invoke(const char * hint);
		static void DumpTable(lua_State*L);
		static int PreloadScript(lua_State*L);
		static bool GetFromCache(lua_State*L, const char*scriptName);
		static bool LoadScriptCode(lua_State*L, const char*scriptName, const char*code, int codeLen);
		static bool GetLoadedTable(lua_State*L, const char *tableName, bool createWhenNotExist = false);
		static bool ExecuteScript(lua_State*L, const char*scriptName, const char*code, int codeLen);
		static bool GetLoadedScript(lua_State*L, const char*scriptName, const char*space=nullptr);
		static bool LoadScriptFromPreloadedBuffers(lua_State*L, const char*scriptName);
		static bool LoadScriptFromUserPath(lua_State*L, const char*scriptName);
		static bool LoadScriptFromAssetsPath(lua_State*L, const char*scriptName);
		static void UnloadScriptCode(lua_State*L, const char*scriptName);
		static const char*GetLuaPathViaGUID(const char*guid);
		static bool LoadInternalScript(lua_State*L, const char*scriptName);
		static void StartFromEntry();
	private:
		static int mStackSize;
		static int mTraceBackFoo;
		static std::stack<lua_State*> mStatePool;
		static std::unordered_map<std::string, std::string> mScriptIDToPath;
	};
}
