#pragma once
#include "Runtime/Physics/Physics2DCircle.h"

namespace Alice
{
	class ExportedPhysics2DCircle :public Physics2DCircle
	{
	public:
		static ExportedPhysics2DCircle*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetRadius);
		DEFINE_LUA_API(GetRadius);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics2DCircle, Physics2DCircle)
	};
}
