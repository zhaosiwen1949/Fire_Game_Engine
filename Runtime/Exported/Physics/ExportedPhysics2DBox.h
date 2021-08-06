#pragma once
#include "Runtime/Physics/Physics2DBox.h"

namespace Alice
{
	class ExportedPhysics2DBox :public Physics2DBox
	{
	public:
		static ExportedPhysics2DBox*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics2DBox, Physics2DBox)
	};
}
