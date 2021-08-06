#include "LuaCallback.h"
#include "Runtime/String/FixedString.h"

namespace Alice {
	Param::Param() {
		mType = kUnkown;
	}
	void Param::PushValue(lua_State*L) {
		switch (mType)
		{
		case kInt:
			lua_pushinteger(L, mIntValue);
			break;
		case kFloat:
			lua_pushnumber(L, mFloatValue);
			break;
		case kString:
			lua_pushstring(L, ((FixedString*)mValue)->mText);
			break;
		}
	}
}