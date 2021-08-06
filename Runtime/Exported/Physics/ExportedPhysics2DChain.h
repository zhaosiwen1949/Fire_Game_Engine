#pragma once
#include "Runtime/Physics/Physics2DChain.h"

namespace Alice
{
	class ExportedPhysics2DChain :public Physics2DChain
	{
	public:
		static ExportedPhysics2DChain*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetVertexCount);
		DEFINE_LUA_API(SetVertex);
		DEFINE_LUA_API(Finish);//create fixture
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics2DChain, Physics2DChain)
	};
}
