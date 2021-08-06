#include "ExportedPhysics2DChain.h"
#include "Runtime/Physics/PhysicsManager.h"

namespace Alice
{
	int ExportedPhysics2DChain::API_SetVertexCount(lua_State*L)
	{
		ExportedPhysics2DChain*ptr = TO_USERDATA(L, ExportedPhysics2DChain, 1);
		ptr->mVertexCount = lua_tointeger(L, 2);
		ptr->mVertexes = new b2Vec2[ptr->mVertexCount];
		return 0;
	}

	int ExportedPhysics2DChain::API_SetVertex(lua_State*L)
	{
		ExportedPhysics2DChain*ptr = TO_USERDATA(L, ExportedPhysics2DChain, 1);
		ptr->mVertexes[lua_tointeger(L, 2)].Set(mWorldScale*lua_tonumber(L, 3), mWorldScale* lua_tonumber(L, 4));
		return 0;
	}
	int ExportedPhysics2DChain::API_Finish(lua_State*L)
	{
		ExportedPhysics2DChain*ptr = TO_USERDATA(L, ExportedPhysics2DChain, 1);
		ptr->mShape.CreateChain(ptr->mVertexes, ptr->mVertexCount);
		ptr->mFixture = ptr->mBody->CreateFixture(&ptr->mShape, 0.0f);
		return 0;
	}
	int ExportedPhysics2DChain::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetVertexCount)
			EXPORT_API(SetVertex)
			EXPORT_API(Finish)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics2DChain, apis);
		SET_PARENT(L, Physics2DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics2DChain, api_c);
		return 0;
	}

	ExportedPhysics2DChain*ExportedPhysics2DChain::New(){
		ExportedPhysics2DChain*p2d = CreateExportedAliceType<ExportedPhysics2DChain>();
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.userData = p2d;
		p2d->mBody = GetPhysicsManager().m2DPhysicWorld->CreateBody(&bodyDef);
		return p2d;
	}
}