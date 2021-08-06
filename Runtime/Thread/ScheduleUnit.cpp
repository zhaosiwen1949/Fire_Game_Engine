#include "ScheduleUnit.h"
#include "Runtime/LuaEngine/LuaEngine.h"
#include "Runtime/Debugger/log.h"

namespace Alice
{
	ScheduleUnit::ScheduleUnit():mFoo(nullptr),mParam(nullptr),mLuaFoo(LUA_REFNIL),mLuaObject(LUA_REFNIL),mLuaParam(LUA_REFNIL),
		mTime(0.0f),mRepeatTime(0), mRepeatInterval(0.0f),mDelayTime(0.0f),mbDeleteAfterInvoke(false)
	{
	}

	bool ScheduleUnit::Update(float deltaTime)
	{
		mTime += deltaTime;
		if (mTime>=mDelayTime)
		{
			Invoke();
			if (mRepeatTime==-1)
			{
				//repeat forever
				mTime = mTime - mRepeatInterval;
				return true;
			}
			else
			{
				mRepeatTime--;
				if (mRepeatTime<=0)
				{
					return false;
				}
				else
				{
					mTime = mTime-mRepeatInterval;
				}
				return true;
			}
		}
		return true;
	}

	void ScheduleUnit::Invoke()
	{
		if (mFoo != nullptr)
		{
			mFoo(mParam);
		}
		else if (mLuaFoo!=LUA_REFNIL)
		{
			lua_State*L = LuaEngine::Store();
			lua_getref(L,mLuaFoo);
			int nArgCount = 0;
			//invoke lua script
			if (mLuaObject!=LUA_REFNIL)
			{
				//member method
				lua_getref(L,mLuaObject);
				nArgCount++;
			}
			if (mLuaParam!=LUA_REFNIL)
			{
				lua_getref(L,mLuaParam);
				nArgCount++;
			}
			if (lua_pcall(L,nArgCount,0,-nArgCount-2)!=0)
			{
				Error("Schedule Unit error happend %s",lua_tostring(L,-1));
			}
			LuaEngine::Restore();
		}
	}

	void ScheduleUnit::Cancel()
	{
		mFoo = nullptr;
		mParam = nullptr;
		mNext = nullptr;
		mRepeatTime = 1;
		mbDeleteAfterInvoke = true;
		if (mLuaFoo != LUA_REFNIL)
		{
			lua_State*L = LuaEngine::Store();
			lua_unref(L, mLuaFoo);
			if (mLuaObject != LUA_REFNIL)
			{
				lua_unref(L, mLuaObject);
			}
			if (mLuaParam != LUA_REFNIL)
			{
				lua_unref(L, mLuaParam);
			}
			mLuaFoo = LUA_REFNIL;
			mLuaObject = LUA_REFNIL;
			mLuaParam = LUA_REFNIL;
			LuaEngine::Restore();
		}
	}

	ScheduleUnit::~ScheduleUnit()
	{
		mFoo = nullptr;
		mParam = nullptr;
		mNext = nullptr;
		mbDeleteAfterInvoke = false;
		if (mLuaFoo!=LUA_REFNIL)
		{
			lua_State*L = LuaEngine::Store();
			lua_unref(L, mLuaFoo);
			if (mLuaObject!=LUA_REFNIL)
			{
				lua_unref(L, mLuaObject);
			}
			if (mLuaParam != LUA_REFNIL)
			{
				lua_unref(L, mLuaParam);
			}
			LuaEngine::Restore();
		}
	}
}