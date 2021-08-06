#include "ExportedScene.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Render/Material.h"
namespace Alice {
	static std::unordered_set<std::string> sProtectedResources,sLoadedAssetLists,sLoadedScriptBundles;
	int ExportedScene::API_SetFPS(lua_State*L) {
		if (lua_gettop(L) != 1)
		{
			return 0;
		}
		if (!lua_isnumber(L, 1))
		{
			return 0;
		}
		GetSceneManager()->mMaxFPS = lua_tointeger(L, 1);
		GetSceneManager()->mFixedTimePerFrame = 1000.0f / (float)GetSceneManager()->mMaxFPS;
		return 0;
	}
	int ExportedScene::API_TimeSinceStartUp(lua_State*L) {
		lua_pushnumber(L, GetSceneManager()->mTimeSinceStartUp);
		return 1;
	}
	int ExportedScene::API_LoadAssetBundle(lua_State*L) {
#if !_DEBUG && !DEV_PUBLISH
		const char * asset_list = lua_tostring(L,1);
		auto iter = sLoadedAssetLists.find(asset_list);
		if (iter!=sLoadedAssetLists.end()){
			return 0;
		}
		if (!Resource::LoadAssetList(lua_tostring(L, 1))) {
			Error("load asset list fail %s", lua_tostring(L, 1));
		}
		else {
			sLoadedAssetLists.insert(asset_list);
		}
#endif
		return 0;
	}
	int ExportedScene::API_UnloadAssetBundle(lua_State*L) {
#if !_DEBUG && !DEV_PUBLISH
		const char * asset_list = lua_tostring(L, 1);
		auto iter = sLoadedAssetLists.find(asset_list);
		if (iter == sLoadedAssetLists.end()) {
			return 0;
		}
		Resource::UnloadAssetList(lua_tostring(L, 1));
		sLoadedAssetLists.erase(iter);
#endif
		return 0;
	}

	int ExportedScene::API_LoadScriptBundle(lua_State*L) {
#if !_DEBUG && !DEV_PUBLISH
		const char * script_bundle = lua_tostring(L, 1);
		auto iter = sLoadedScriptBundles.find(script_bundle);
		if (iter != sLoadedScriptBundles.end()) {
			return 0;
		}
		if (false == LuaEngine::LoadScriptBundle(lua_tostring(L, 1))) {
			Error("load script bundle fail %s",script_bundle);
		}
		else {
			sLoadedScriptBundles.insert(script_bundle);
		}
#endif
		return 0;
	}

	int ExportedScene::API_UnloadScriptBundle(lua_State*L) {
#if !_DEBUG && !DEV_PUBLISH
		const char * script_bundle = lua_tostring(L, 1);
		auto iter = sLoadedScriptBundles.find(script_bundle);
		if (iter == sLoadedScriptBundles.end()) {
			return 0;
		}
		LuaEngine::UnloadScriptBundle(lua_tostring(L, 1));
		sLoadedScriptBundles.erase(iter);
#endif
		return 0;
	}
	int ExportedScene::API_Protect(lua_State*L) {
		const char *name = lua_tostring(L, 1);
		auto iter = sProtectedResources.find(name);
		if (iter!=sProtectedResources.end()){
			return 0;
		}
		sProtectedResources.insert(name);
		return 0;
	}
	int ExportedScene::API_EnableScenePostRendering(lua_State*L) {
		bool enable = (bool)lua_toboolean(L, 1);
		Material*material = nullptr;
		if (lua_gettop(L) == 2) {
			material = TO_USERDATA(L, Material, 2);
		}
		GetSceneManager()->EnableScenePostRendering(enable, material);
		return 0;
	}
	int ExportedScene::API_EnableUIPostRendering(lua_State*L) {
		bool enable = (bool)lua_toboolean(L, 1);
		Material*material = nullptr;
		if (lua_gettop(L) == 2) {
			material = TO_USERDATA(L, Material, 2);
		}
		GetSceneManager()->EnableUIPostRendering(enable, material);
		return 0;
	}
	int ExportedScene::API_SetFixedUpdateTime(lua_State*L) {
		GetPhysicsManager().mFixedUpdateTime = lua_tonumber(L, 1);
		return 0;
	}
	int ExportedScene::API_SetUIMaxScale(lua_State*L) {
		GetSceneManager()->mUIMaxScale = lua_tonumber(L, 1);
		return 0;
	}
	int ExportedScene::API_GetUIMaxScale(lua_State*L) {
		lua_pushnumber(L, GetSceneManager()->mUIMaxScale);
		return 1;
	}
	int ExportedScene::API_SetOnException(lua_State*L) {
		GetSceneManager()->mOnException = luaL_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}
	int ExportedScene::Export(lua_State*L) {
		APPEND_LUA_PACKAGE_API(L, "Alice", "SetFPS", API_SetFPS);
		APPEND_LUA_PACKAGE_API(L, "Alice", "SetFixedUpdateTime", API_SetFixedUpdateTime);
		APPEND_LUA_PACKAGE_API(L, "Alice", "TimeSinceStartUp", API_TimeSinceStartUp);
		APPEND_LUA_PACKAGE_API(L, "Alice", "LoadScriptBundle", API_LoadScriptBundle);
		APPEND_LUA_PACKAGE_API(L, "Alice", "UnloadScriptBundle", API_UnloadScriptBundle);
		APPEND_LUA_PACKAGE_API(L, "Alice", "LoadAssetBundle", API_LoadAssetBundle);
		APPEND_LUA_PACKAGE_API(L, "Alice", "UnloadAssetBundle", API_UnloadAssetBundle);
		APPEND_LUA_PACKAGE_API(L, "Alice", "Protect", API_Protect);
		APPEND_LUA_PACKAGE_API(L, "Alice", "EnableAA", API_EnableScenePostRendering);
		APPEND_LUA_PACKAGE_API(L, "Alice", "EnablePostRendering", API_EnableScenePostRendering);
		APPEND_LUA_PACKAGE_API(L, "Alice", "EnableScenePostRendering", API_EnableScenePostRendering);
		APPEND_LUA_PACKAGE_API(L, "Alice", "EnableUIPostRendering", API_EnableUIPostRendering);
		APPEND_LUA_PACKAGE_API(L, "Alice", "SetUIMaxScale", API_SetUIMaxScale);
		APPEND_LUA_PACKAGE_API(L, "Alice", "GetUIMaxScale", API_GetUIMaxScale);
		APPEND_LUA_PACKAGE_API(L, "Alice", "SetOnException", API_SetOnException);
		return 0;
	}
}