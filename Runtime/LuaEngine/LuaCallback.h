#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Utils/LinkedList.h"
namespace Alice {
	class LuaCallback {
	public:
		int mLuaTable;
		int mLuaFoo;
	};

	class Param : public LinkedList {
	public:
		enum
		{
			kInt,
			kFloat,
			kString,
			kUnkown
		};
		int mType;
		void*mValue;
		int mIntValue;
		float mFloatValue;
		Param();
		void PushValue(lua_State*L);
	};
}