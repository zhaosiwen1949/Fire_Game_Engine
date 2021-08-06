#pragma once
#include "Runtime/Physics/Physics2DEdge.h"

namespace Alice
{
	class ExportedPhysics2DEdge :public Physics2DEdge
	{
	public:
		static ExportedPhysics2DEdge*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetVertex);
		DEFINE_LUA_API(GetVertex);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics2DEdge, Physics2DEdge)
	};
}
