#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Utils/Timer.h"
namespace Alice{
	class ExportedTimer :public Timer {
	public:
		DEFINE_LUA_API(Reset);
		DEFINE_LUA_API(GetTime);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD_MANAGED(ExportedTimer,Timer)
	};
}
