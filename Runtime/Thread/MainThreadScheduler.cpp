#include "MainThreadScheduler.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Debugger/log.h"

#pragma  warning(disable:4244)
namespace Alice
{
	ScheduleUnit*MainThreadScheduler::mHeadNode[2] = {nullptr,nullptr}, *MainThreadScheduler::mEndNode[2] = { nullptr,nullptr };
	static int sCurrentQueue = 0;

	int MainThreadScheduler::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(CallM)//call member function in main thread
			EXPORT_API(CallS)//call static function in main thread
			EXPORT_API(Cancel)//call static function in main thread
			{NULL,NULL}
		};
		return 0;
	}

	int MainThreadScheduler::API_CallM(lua_State*L)
	{
		//lua object
		//method
		ScheduleUnit*scheduleUnit = new (kMemDefaultId)ScheduleUnit;
		int nArgCount = lua_gettop(L);
		if (nArgCount ==2)
		{
			//invoke next frame for one time
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
			scheduleUnit->mLuaObject = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//param
		else if(nArgCount==3)
		{
			//invoke next frame for once time with one param
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
			scheduleUnit->mLuaObject = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//delay time : 0 ,delay one frame else delay some time
		else if (nArgCount==4)
		{
			//invoke with a delay time
			scheduleUnit->mDelayTime = lua_tonumber(L,-1);
			lua_pop(L, 1);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
			scheduleUnit->mLuaObject = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//repeat time : 0 not repeat,-1 repeat forever otherwise repeat time
		else if (nArgCount==5)
		{
			//repeat invoke after delay time for every frame
			scheduleUnit->mRepeatTime = lua_tointeger(L, -1);
			scheduleUnit->mDelayTime = lua_tonumber(L, -2);
			lua_pop(L, 2);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
			scheduleUnit->mLuaObject = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//repeat interval
		else if (nArgCount==6)
		{
			//repeat invoke after delay time with some certain time interval
			scheduleUnit->mRepeatInterval = lua_tonumber(L, -1);
			scheduleUnit->mRepeatTime = lua_tointeger(L, -2);
			scheduleUnit->mDelayTime = lua_tonumber(L, -3);
			lua_pop(L, 3);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
			scheduleUnit->mLuaObject = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		scheduleUnit->mbDeleteAfterInvoke = true;
		RunInMainThread(scheduleUnit);
		lua_pushlightuserdata(L, scheduleUnit);
		return 1;
	}

	int MainThreadScheduler::API_CallS(lua_State*L)
	{
		//method
		ScheduleUnit*scheduleUnit = new (kMemDefaultId)ScheduleUnit;
		int nArgCount = lua_gettop(L);
		if (nArgCount == 1)
		{
			//invoke next frame for one time 
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//param
		else if (nArgCount == 2)
		{
			//invoke next frame for once time with one param
			if (!lua_istable(L,-1))
			{
				lua_pop(L,1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//delay time : 0 ,delay one frame else delay some time
		else if (nArgCount == 3)
		{
			//invoke with a delay time
			scheduleUnit->mDelayTime = lua_tonumber(L, -1);
			lua_pop(L, 1);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//repeat time : 0 not repeat,-1 repeat forever otherwise repeat time
		else if (nArgCount == 4)
		{
			//repeat invoke after delay time for every frame
			scheduleUnit->mRepeatTime = lua_tointeger(L, -1);
			scheduleUnit->mDelayTime = lua_tonumber(L, -2);
			lua_pop(L, 2);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		//repeat interval
		else if (nArgCount == 5)
		{
			//repeat invoke after delay time with some certain time interval
			scheduleUnit->mRepeatInterval = lua_tonumber(L, -1);
			scheduleUnit->mRepeatTime = lua_tointeger(L, -2);
			scheduleUnit->mDelayTime = lua_tonumber(L, -3);
			lua_pop(L, 3);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
			}
			else
			{
				scheduleUnit->mLuaParam = luaL_ref(L, LUA_REGISTRYINDEX);
			}
			scheduleUnit->mLuaFoo = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		scheduleUnit->mbDeleteAfterInvoke = true;
		RunInMainThread(scheduleUnit);
		lua_pushlightuserdata(L, scheduleUnit);
		return 1;
	}

	int MainThreadScheduler::API_Cancel(lua_State*L)
	{
		if (lua_islightuserdata(L,1))
		{
			//stop schedule with something
			ScheduleUnit*ptr = (ScheduleUnit*)lua_touserdata(L, 1);
			ptr->Cancel();
		}
		return 0;
	}

	ScheduleUnit* MainThreadScheduler::RunInMainThread(schedulefoo foo, void*param)
	{
		ScheduleUnit*scheduleUnit = new (kMemDefaultId)ScheduleUnit;
		scheduleUnit->mFoo = foo;
		scheduleUnit->mParam = param;
		scheduleUnit->mbDeleteAfterInvoke = true;
		RunInMainThread(scheduleUnit);
		return scheduleUnit;
	}

	void MainThreadScheduler::RunInMainThread(ScheduleUnit*scheduleUnit)
	{
		if (mHeadNode[sCurrentQueue] == nullptr)
		{
			mHeadNode[sCurrentQueue] = scheduleUnit;
		}
		else
		{
			mHeadNode[sCurrentQueue]->PushBack(scheduleUnit);
		}
		mEndNode[sCurrentQueue] = scheduleUnit;
	}

	void MainThreadScheduler::Update(float deltaTime)
	{
		if (mHeadNode[sCurrentQueue]==nullptr)
		{
			return;
		}
		int currentQueue = sCurrentQueue;

		int nextQueue = (sCurrentQueue + 1) % 2;
		sCurrentQueue = nextQueue;
		mHeadNode[sCurrentQueue] = nullptr;
		mEndNode[sCurrentQueue] = nullptr;

		ScheduleUnit*next = mHeadNode[currentQueue];
		while (next !=nullptr)
		{
			ScheduleUnit*current = next;
			next = next->mNext==nullptr?nullptr: next->Next<ScheduleUnit>();
			current->mNext = nullptr;
			if (!current->Update(deltaTime))
			{
				if(current->mbDeleteAfterInvoke) delete current;
			}
			else
			{
				RunInMainThread(current);
			}
		}
	}
}