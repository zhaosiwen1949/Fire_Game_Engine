#pragma once
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/String/FixedString.h"
namespace Alice
{
	typedef void(*schedulefoo)(void*);
	class ScheduleUnit :public LinkedList
	{
	public:
		ScheduleUnit();
		~ScheduleUnit();
		schedulefoo mFoo;
		int mLuaObject;
		int mLuaFoo;
		int mLuaParam;
		float mDelayTime;
		float mTime;
		float mRepeatInterval;
		int mRepeatTime;
		void*mParam;
		bool mbDeleteAfterInvoke;
		void Invoke();
		void Cancel();
		bool Update(float deltaTime);
	};
}