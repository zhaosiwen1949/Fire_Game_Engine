#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Scene/SceneManager.h"
namespace Alice {
	class ExportedScene : public SceneManager {
	public:
		DEFINE_LUA_API(SetFPS);
		DEFINE_LUA_API(SetFixedUpdateTime);
		DEFINE_LUA_API(TimeSinceStartUp);
		DEFINE_LUA_API(Protect);
		DEFINE_LUA_API(LoadScriptBundle);
		DEFINE_LUA_API(UnloadScriptBundle);
		DEFINE_LUA_API(LoadAssetBundle);
		DEFINE_LUA_API(UnloadAssetBundle);
		DEFINE_LUA_API(EnableAA);
		DEFINE_LUA_API(EnableScenePostRendering);
		DEFINE_LUA_API(EnableUIPostRendering);
		DEFINE_LUA_API(SetUIMaxScale);
		DEFINE_LUA_API(GetUIMaxScale);
		DEFINE_LUA_API(SetOnException);
		static int Export(lua_State*L);
	};
}
