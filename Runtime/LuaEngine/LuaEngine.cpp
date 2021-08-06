#include "LuaEngine.h"
#include "Runtime/Debugger/Log.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Exported/ExportModules.h"
#include "Runtime/Scene/SceneManager.h"
#if ALICE_PLATFORM_WIN
#pragma comment(lib,"Lua5.1.4.lib")
#pragma comment(lib,"LibProtoBufferLite.lib")
#pragma comment(lib,"LibPBC.lib")
#pragma comment(lib,"LuaSocket.lib")
#endif
static char * sEntry = nullptr;
static char * sEntryScriptBuddle = nullptr;
lua_State*Alice::LuaEngine::s_GlobalStatePtr = nullptr;
int Alice::LuaEngine::mCurrentArgCount = 0;
extern "C" void SetEntryScript(const char * entry_script_bundle,const char * init_script) {
	if (entry_script_bundle != nullptr) {
		sEntryScriptBuddle = new char[256];
		memset(sEntryScriptBuddle, 0, 256);
		strcpy(sEntryScriptBuddle, entry_script_bundle);
	}
	if (init_script != nullptr) {
		sEntry = new char[256];
		memset(sEntry, 0, 256);
		strcpy(sEntry, init_script);
	}
}
void*AliceEngine_GetLuaVM() {
	return Alice::LuaEngine::s_GlobalStatePtr;
}

namespace Alice
{
	int LuaEngine::mTraceBackFoo = LUA_REFNIL;
	std::stack<lua_State*> LuaEngine::mStatePool;
	std::unordered_map<std::string, std::string> LuaEngine::mScriptIDToPath;
	void LuaEngine::Init() {
		s_GlobalStatePtr = luaL_newstate();
		luaL_openlibs(s_GlobalStatePtr);
		ExportAliceRuntime();
		lua_getglobal(s_GlobalStatePtr, "debug");
		lua_getfield(s_GlobalStatePtr, -1, "traceback");
		mTraceBackFoo = luaL_ref(s_GlobalStatePtr, LUA_REGISTRYINDEX);
		lua_settop(s_GlobalStatePtr, 0);
		Info("alice engine init sucess with code %d", lua_gettop(s_GlobalStatePtr));
	}
	lua_State* LuaEngine::GetLuaState()
	{
		lua_State*ret = mStatePool.top();
		mStatePool.pop();
		return ret;
	}

	int LuaEngine::mStackSize = 0;
	static std::stack<int> mLuaStackState;
	lua_State* LuaEngine::Store() {
		mLuaStackState.push(lua_gettop(s_GlobalStatePtr));
		lua_getref(s_GlobalStatePtr, mTraceBackFoo);//push traceback foo to stack
		return s_GlobalStatePtr;
	}
	int LuaEngine::Restore() {
		int topSize = mLuaStackState.top();
		lua_settop(s_GlobalStatePtr, topSize);
		mLuaStackState.pop();
		return topSize;
	}
	bool LuaEngine::PrepareMethod(int foo) {
		lua_State*luaEngine = Store();
		lua_getref(luaEngine, foo);
		if (lua_isfunction(luaEngine, -1)) {
			mCurrentArgCount = 0;
			return true;
		}
		Restore();
		return false;
	}
	void LuaEngine::PushInt(int a) {
		mCurrentArgCount++;
		lua_pushinteger(s_GlobalStatePtr, a);
	}
	void LuaEngine::PushBoolean(bool a) {
		mCurrentArgCount++;
		lua_pushboolean(s_GlobalStatePtr, a);
	}
	void LuaEngine::PushNumber(double a) {
		mCurrentArgCount++;
		lua_pushnumber(s_GlobalStatePtr, a);
	}
	void LuaEngine::PushString(const char *str) {
		mCurrentArgCount++;
		lua_pushstring(s_GlobalStatePtr, str);
	}
	void LuaEngine::PushLString(const char *str, int len) {
		mCurrentArgCount++;
		lua_pushlstring(s_GlobalStatePtr, str, len);
	}
	void LuaEngine::PushNil(int count/* =1 */) {
		while (count>0){
			lua_pushnil(s_GlobalStatePtr);
			mCurrentArgCount++;
			count--;
		}
	}
	void LuaEngine::PushUserData(int userdata) {
		mCurrentArgCount++;
		if (userdata==LUA_REFNIL){
			lua_pushnil(s_GlobalStatePtr);
		}
		else {
			lua_getref(s_GlobalStatePtr, userdata);
		}
	}
	void LuaEngine::NewTable() {
		mCurrentArgCount++;
		lua_newtable(s_GlobalStatePtr);
	}
	void LuaEngine::Invoke(const char * hint) {
		int error_foo_index = -2 - mCurrentArgCount;
		if (lua_pcall(s_GlobalStatePtr, mCurrentArgCount, 0, error_foo_index) != 0) {
			Error(hint, lua_tostring(s_GlobalStatePtr, -1));
		}
		mCurrentArgCount = 0;
		Restore();
	}
#if ALICE_PLATFORM_WIN
	static std::unordered_map<std::string, HMODULE> sPlugins;
#endif
	int LuaEngine::CallLibMethod(lua_State*L)
	{
		if (!lua_isstring(L,1))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "first arg must be a string value to specify lib path relative to Assets Folder");
			return 2;
		}
		if (!lua_isstring(L,2))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "second arg must be a string value to specify method name");
			return 2;
		}
#if ALICE_PLATFORM_WIN
		const char*libName = lua_tostring(L, 1);
		const char*method = lua_tostring(L, 2);
		std::unordered_map<std::string, HMODULE>::iterator iter = sPlugins.find(libName);
		HMODULE module = nullptr;
		if (iter==sPlugins.end())
		{
			module = LoadLibraryA(libName);
			if (module!=nullptr)
			{
				sPlugins.insert(std::pair<std::string,HMODULE>(libName,module));
			}
			else
			{
				lua_pushboolean(L, false);
				lua_pushstring(L, "load lib fail");
				return 1;
			}
		}
		else
		{
			module = iter->second;
		}
		lua_CFunction foo = (lua_CFunction)GetProcAddress(module, method);
		if (foo != nullptr)
		{
			lua_pushcfunction(L, foo);
			if (lua_pcall(L, 0, 0, 0) != 0)
			{
				lua_pushboolean(L, false);
				lua_pushvalue(L, -2);
			}
			else
			{
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
			}
		}
		else
		{
			lua_pushboolean(L, false);
			char szBuffer[256] = {0};
			sprintf(szBuffer,"cannot find %s in lib %s",method,libName);
			lua_pushstring(L, szBuffer);
		}
#endif
		return 2;
	}

	void LuaEngine::CleanPlugins()
	{
#if ALICE_PLATFORM_WIN
		std::unordered_map<std::string, HMODULE>::iterator iter = sPlugins.begin();
		for (;iter!=sPlugins.end();)
		{
			FreeLibrary(iter->second);
			iter = sPlugins.erase(iter);
		}
#endif
	}

	void LuaEngine::RecycleLuaState(lua_State*L)
	{
		mStatePool.push(L);
	}

	bool LuaEngine::GetLoadedScript(lua_State*L, const char*scriptName, const char*space) {
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		if (space==nullptr){
			lua_getfield(L, -1, scriptName);
			if (!lua_isnil(L, -1)) {
				lua_remove(L, -2);
				return true;
			}
			else {
				lua_pop(L, 2);
			}
		}
		else {
			lua_getfield(L, -1, space);
			if (lua_istable(L, -1)) {
				lua_getfield(L, -1, scriptName);
				if (lua_isnil(L, -1)) {
					lua_pop(L, 3);
				}
				else {
					lua_remove(L, -2);
					lua_remove(L, -2);
					return true;
				}
			} else {
				lua_pop(L, 2);
			}
		}
		return false;
	}
	bool LuaEngine::GetFromCache(lua_State*L, const char*scriptName)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_getfield(L, -1, "Assets");
		if (lua_istable(L, -1)){
			lua_getfield(L, -1, scriptName);
			if (lua_isnil(L, -1)){
				lua_pop(L, 3);
			}else{
				lua_remove(L, -2);
				lua_remove(L, -2);
				return true;
			}
		}else{
			lua_pop(L, 2);
		}
		return false;
	}

	bool LuaEngine::LoadScriptFromPreloadedBuffers(lua_State*L, const char*scriptName) {
#if ALICE_EDITOR || (ALICE_WIN_PLAYER && _DEBUG)|| (ALICE_OSX_PLAYER && _DEBUG) || DEV_PUBLISH
#else
		const Serializer::Script *code = LuaEngine::GetLuaBuffer(scriptName);
		if (code != nullptr){
			LoadScriptCode(L, code->name().c_str(), code->code().c_str(), code->code().length());
			return true;
		}else{
			errorC("cannot locate script (runtime) %s", scriptName);
		}
#endif
		return false;
	}
	bool LuaEngine::LoadScriptFromUserPath(lua_State*L, const char*scriptName) {
		char luaPath[256] = { 0 };
		strcpy(luaPath, scriptName);
		StringUtils::LuaPathToSTDPath(luaPath);
		strcat(luaPath, ".lua");
		Data luaSrc;
		if (FileSystem::LoadDataFromPath(luaPath, luaSrc)) {
			LoadScriptCode(L, scriptName, (char*)luaSrc.mData, luaSrc.mDataLen);
			return true;
		}
		return false;
	}
	bool LuaEngine::LoadScriptFromAssetsPath(lua_State*L, const char*scriptName) {
		char luaPath[256] = { 0 };
		strcpy(luaPath, scriptName);
		StringUtils::LuaPathToSTDPath(luaPath);
		strcat(luaPath, ".lua");
		Data luaSrc;
		if (ResourceManager::LoadInternalData(luaPath, luaSrc)) {
			LoadScriptCode(L, scriptName, (char*)luaSrc.mData, luaSrc.mDataLen);
			return true;
		}
		return false;
	}
	void LuaEngine::UnloadScriptCode(lua_State*L, const char*scriptName){
		Store();
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");//package.loaded
		lua_getfield(L, -1, "Assets");
		lua_pushnil(L);
		lua_setfield(L, -2, scriptName);
		Restore();
	}

	bool LuaEngine::GetLoadedTable(lua_State*L, const char *tableName, bool createWhenNotExist /* = false */) {
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");//package.loaded
		lua_getfield(L, -1, "Assets");
		if (lua_isnil(L, -1)&&createWhenNotExist) {
			//create Assets table if not exist
			lua_pop(L, 1);
			lua_newtable(L);
			lua_setfield(L, -2, "Assets");
		} else {
			return false;
		}
		return true;
	}

	bool LuaEngine::ExecuteScript(lua_State*L, const char*scriptName, const char*code, int codeLen) {
		if (luaL_loadbuffer(L, code, codeLen, scriptName) == 0) {
			if (lua_pcall(L, 0, 1, -4) == 0) {
				if (lua_istable(L, -1)) {
					lua_pushstring(L, scriptName);
					lua_setfield(L, -2, "Name");//{}.Name=scriptname
					lua_setfield(L, -2, scriptName);//package.loaded.assets.scriptname={}
					lua_getfield(L, -1, scriptName);
					lua_remove(L, -2);//remove Assets table
					lua_remove(L, -2);//remove package.loaded table
					lua_remove(L, -2);//remove traceback
				}
				else {//the scripts not return a table
					  //remove traceback(-4),package.loaded table(-3),Assets table(-2),nil(-1)
					lua_pop(L, 4);
					lua_pushnil(L);
				}
				return true;
			}
			else {
				errorC("error on load script code excution %s : %s", scriptName, lua_tostring(L, -1));
				//remove traceback(-4),package.loaded table(-3),Assets table(-2),err_string(-1)
				lua_pop(L, 4);
				lua_pushnil(L);
			}
		}
		else {
			errorC("error on load script code %s : %s", scriptName, lua_tostring(L, -1));
			//remove traceback(-4),package.loaded table(-3),Assets table(-2),err_string(-1)
			lua_pop(L, 4);
			lua_pushnil(L);
		}
		return false;
	}

	bool LuaEngine::LoadScriptCode(lua_State*L, const char*scriptName, const char*code, int codeLen) {
		lua_getref(L, mTraceBackFoo);
		GetLoadedTable(L, "Assets", true);
		return ExecuteScript(L, scriptName, code, codeLen);
	}

	int LuaEngine::PreloadScript(lua_State*L)
	{
		if (lua_gettop(L)!=2)
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "arg count must be 2");
			return 2;
		}
		if (!lua_isstring(L,1)||
			!lua_isstring(L,2))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "arg must be 2 string");
			return 2;
		}
		const char*scriptName = lua_tostring(L,1);
		const char*code = lua_tostring(L,2);
		LoadScriptCode(L, scriptName, code,strlen(code));
		return 1;
	}
	const char* LuaEngine::GetLuaPathViaGUID(const char*guid)
	{
		std::unordered_map<std::string, std::string>::iterator iter = mScriptIDToPath.find(guid);
		if (iter != mScriptIDToPath.end())
		{
			return iter->second.c_str();
		}
		return nullptr;
	}
	bool LuaEngine::LoadInternalScript(lua_State*L, const char*scriptName) {
		//input is a script name like xx.xx.xx
		//return a lua table if load script sucess,if fail lua stack size unchanged
		if (GetFromCache(L, scriptName)) {
			return true;
		}
		char luaPath[256] = { 0 };
		strcpy(luaPath, scriptName);
		StringUtils::LuaPathToSTDPath(luaPath);
		strcat(luaPath, ".lua");
		Data luaSrc;
		if (ResourceManager::LoadData(luaPath, luaSrc)) {
			LoadScriptCode(L, scriptName, (char*)luaSrc.mData, luaSrc.mDataLen);
		}
		else {
			errorC("cannot locate script %s : %s", scriptName, luaPath);
			return false;
		}
		return true;
	}
	void LuaEngine::StartFromEntry() {
		lua_State*L = Store();
		if (sEntryScriptBuddle != nullptr) {
		}
		if (sEntry != nullptr) {
			LoadInternalScript(L, sEntry);
		}
		Restore();
		GetSceneManager()->Play();
	}

	void LuaEngine::DumpTable(lua_State*L)
	{
		if (!lua_istable(L,-1))
		{
			Error("top is not a table");
			return;
		}
		lua_pushnil(L);  /* first key */
		while (lua_next(L, -2)) {
			lua_getglobal(L,"tostring");
			lua_pushvalue(L, -3);
			const char*key = "unknown";
			if (lua_pcall(L, 1, 1, 0) == 0)
			{
				key = lua_tostring(L,-1);
			}
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -3); 
			const char*value = "unknown";
			if (lua_pcall(L, 1, 1, 0) == 0)
			{
				value = lua_tostring(L, -1);
			}
			Debug("%s : %s", key,value);
			lua_pop(L, 3);
		}
	}
}
