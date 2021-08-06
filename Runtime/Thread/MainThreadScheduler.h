#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "ScheduleUnit.h"
namespace Alice{
	class MainThreadScheduler{
	public:
		static ScheduleUnit*RunInMainThread(schedulefoo foo, void*param);
		static void RunInMainThread(ScheduleUnit*scheduleUnit);
		static void Update(float deltaTime);
		static int Export(lua_State*L);
		DEFINE_LUA_API(CallM);
		DEFINE_LUA_API(CallS);
		DEFINE_LUA_API(Cancel);
		static ScheduleUnit *mHeadNode[2],*mEndNode[2];
	};
}
