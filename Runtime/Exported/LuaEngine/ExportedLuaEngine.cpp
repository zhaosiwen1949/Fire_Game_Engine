#include "ExportedLuaEngine.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Utils/SystemInfo.h"
#include "Runtime/Device/DeviceInfo.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ResourceManager.h"

#if ALICE_IPHONE
extern "C" void ShowiPhoneSoftKeyboard(const char *utf8Str);
#elif ALICE_ANDROID
#include"Runtime/Android/JNIUtils.h"
#endif
extern "C" void OnQuitEngine();
static int sCoreVersion = 1;
extern "C" void SetCoreVersion(int version) {
	sCoreVersion = version;
}
namespace Alice {
	int ExportedLuaEngine::API_require(lua_State*L) {
		if (lua_gettop(L) != 1){
			lua_pushnil(L);
			return 1;
		}
		if (!lua_isstring(L, 1)){
			lua_pushnil(L);
			return 1;
		}
		const char*script = lua_tostring(L, 1);
		lua_getglobal(L, script);//find script in global namespace
		if (!lua_isnil(L, -1)){
			return 1;
		}
		lua_pop(L, 1);
		if (GetLoadedScript(L,script)){//find script in _LOADED namespace
			return 1;
		}
		if (GetLoadedScript(L, script,"Assets")) {//find script in _LOADED.Assets
			return 1;
		}
		//load script from preloaded scripts,�ܴ�����������ص��Ĵ��붼�ǲ�Ʒ��ͨ��LoadScriptBundleԤ���صĴ���
		if (LoadScriptFromPreloadedBuffers(L, script)){
			return 1;
		}
		//load script from Assets folder�����Դ�AssetsĿ¼�¼��ش���
		if (LoadScriptFromAssetsPath(L, script)) {
			return 1;
		}
		//���ֱ�ӳ��Դ��û�ָ����Ŀ¼���ش��룬���Դ���Ĳ������κε����κ͸���
		if (LoadScriptFromUserPath(L,script)){
			return 1;
		}
		Error("require script %s fail",script);
		return 1;
	}

	int ExportedLuaEngine::API_JumpTo(lua_State*L) {
#ifndef _DEBUG
#endif
		return 0;
	}
	int ExportedLuaEngine::sUserPrintAgent = LUA_REFNIL;
	int ExportedLuaEngine::sUserErrorAgent = LUA_REFNIL;
	int ExportedLuaEngine::API_SetUserPrintAgent(lua_State*L) {
		sUserErrorAgent = lua_ref(L, LUA_REGISTRYINDEX);
		sUserPrintAgent = lua_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}
	int ExportedLuaEngine::API_UserPrint(lua_State*L) {
		int nCount = lua_gettop(L);
		int nIndex = 0;
		char szLogBuffer[1024] = {0};
		int nLen = 0;
		while (nIndex < nCount) {
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, nIndex+1);
			lua_pcall(L, 1, 1, 0);
			const char*log = lua_tostring(L, -1);
			nLen += (strlen(log) + 1);
			if (nLen > 1024) {
				break;
			}
			strcat(szLogBuffer, log);
			strcat(szLogBuffer, " ");
			nIndex++;
		}
		if (sUserPrintAgent!=LUA_REFNIL){
			lua_State * edu_state = Store();
			lua_getref(edu_state, sUserPrintAgent);
			lua_pushlstring(edu_state, szLogBuffer, nLen);
			if (lua_pcall(edu_state, 1, 0, -3)!=0) {
				Error("%s",lua_tostring(edu_state,-1));
			}
			Restore();
		}
		else {
			Debug("%s", szLogBuffer);
		}
		return 0;
	}
	void ExportedLuaEngine::ExecuteUserScript(lua_State*L, const char*scriptName, const char*code, int codeLen) {
		lua_pushcfunction(L, API_UserPrint);
		lua_setglobal(L, "print");
		lua_getglobal(L, "debug");
		lua_getfield(L, -1, "traceback");
		if (luaL_loadbuffer(L, code, codeLen, scriptName) == 0) {
			if (lua_pcall(L, 0, 1, -2) != 0) {
				if (sUserErrorAgent != LUA_REFNIL) {
					lua_State * edu_state = Store();
					lua_getref(edu_state, sUserErrorAgent);
					lua_pushstring(edu_state, lua_tostring(L, -1));
					if (lua_pcall(edu_state, 1, 0, -3) != 0) {
						Error("%s", lua_tostring(edu_state, -1));
					}
					Restore();
				}
			}
		}
		else {
			lua_State * edu_state = Store();
			lua_getref(edu_state, sUserErrorAgent);
			lua_pushstring(edu_state, lua_tostring(L, -1));
			if (lua_pcall(edu_state, 1, 0, -3) != 0) {
				Error("%s", lua_tostring(edu_state, -1));
			}
			Restore();
		}
	}
	int ExportedLuaEngine::API_EvalUserScript(lua_State*L) {
		lua_State *user_state = luaL_newstate();
		luaL_openlibs(user_state);
		const char*scriptName = lua_tostring(L, 1);
		const char*code = lua_tostring(L, 2);
		int codeLen = lua_tointeger(L, 3);
		ExecuteUserScript(user_state, scriptName, code, codeLen);
		lua_close(user_state);
		return 0;
	}

	int ExportedLuaEngine::API_CoreVersion(lua_State*L) {
		lua_pushinteger(L, sCoreVersion);
		return 1;
	}

	int ExportedLuaEngine::API_ShowKeyboard(lua_State*L) {
		const char *str = "";
		if (lua_gettop(L) == 1 && lua_isstring(L, 1)) {
			str = lua_tostring(L, 1);
		}
#if ALICE_IPHONE
		ShowiPhoneSoftKeyboard(str);
#elif ALICE_ANDROID
		ShowSoftKeyboard(str);
#endif
		return 0;
	}

	int ExportedLuaEngine::API_HideKeyboard(lua_State*L) {
#if ALICE_IPHONE
		HideiPhoneSoftKeyboard();
#elif ALICE_ANDROID
		HideSoftKeyboard();
#endif
		return 0;
	}

	int ExportedLuaEngine::API_Exit(lua_State*L) {
		OnQuitEngine();
		exit(0);
		return 0;
	}

	int ExportedLuaEngine::API_IsExpVersion(lua_State*L) {
		return 0;
	}
	int ExportedLuaEngine::API_GC(lua_State*L) {
		if (lua_gettop(L)==0){
			lua_gc(L, LUA_GCCOLLECT, 0);
		}
		return 0;
	}
#if ALICE_ANDROID
	int API_InstallApk(lua_State*L){
		InstallApk(lua_tostring(L,1));
		return 0;
	}
#endif
#ifdef ALICE_WIN_PLAYER
	int ExportedLuaEngine::API_CHDIR(lua_State*L) {
		const char *utf8Path = lua_tostring(L, 1);
		if (Alice::FileSystem::Exists(utf8Path)) {
			SetCurrentDirectoryA(utf8Path);
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}
#endif
	int ExportedLuaEngine::Export(lua_State*L)
	{
		lua_pushcfunction(L, API_require);
		lua_setglobal(s_GlobalStatePtr, "require");
		luaL_Reg api_c[] = {
			{ "Register",CallLibMethod },
			{ "RuntimePlatform",SystemInfo::RuntimePlatform },
			{ "AssetsPath",ResourceManager::AssetsPath },
			{ "DataPath",ResourceManager::DataPath },
			{ "DocPath",ResourceManager::DocPath },
			{ "DesktopPath",ResourceManager::DesktopPath },
			{ "OpenURL",SystemInfo::OpenURL },
#if ALICE_ANDROID
			EXPORT_API(InstallApk)
#endif
			{ "PreloadScript",PreloadScript },
			{ "SetOnViewportChanged",PreloadScript },
			EXPORT_API(EvalUserScript)
			EXPORT_API(SetUserPrintAgent)
			EXPORT_API(ShowKeyboard)
			EXPORT_API(HideKeyboard)
			EXPORT_API(CoreVersion)
			EXPORT_API(Exit)
			EXPORT_API(GC)
#if _DEBUG || DEV_PUBLISH
			EXPORT_API(IsExpVersion)
#endif
#ifdef ALICE_WIN_PLAYER
			EXPORT_API(CHDIR)
#endif
			{ NULL,NULL }
		};
		luaL_openlib(L, "Alice", api_c, 0);
		return 0;
	}
}
