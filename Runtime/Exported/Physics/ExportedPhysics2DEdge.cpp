#include "ExportedPhysics2DEdge.h"
#include "Runtime/Physics/PhysicsManager.h"

namespace Alice
{
	int ExportedPhysics2DEdge::API_SetVertex(lua_State*L)
	{
		ExportedPhysics2DEdge*ptr = TO_USERDATA(L, ExportedPhysics2DEdge, 1);
		b2Vec2 v1(lua_tonumber(L,2)*mWorldScale, lua_tonumber(L, 3)*mWorldScale), v2(lua_tonumber(L, 4)*mWorldScale, lua_tonumber(L, 5)*mWorldScale);
		ptr->mShape.Set(v1, v2);
		ptr->mFixture=ptr->mBody->CreateFixture(&ptr->mShape, 0.0f);
		return 0;
	}

	int ExportedPhysics2DEdge::API_GetVertex(lua_State*L)
	{
		ExportedPhysics2DEdge*ptr = TO_USERDATA(L, ExportedPhysics2DEdge, 1);
		lua_pushnumber(L, ptr->mShape.m_vertex1.x / mWorldScale);
		lua_pushnumber(L, ptr->mShape.m_vertex1.y / mWorldScale);
		lua_pushnumber(L, ptr->mShape.m_vertex2.x / mWorldScale);
		lua_pushnumber(L, ptr->mShape.m_vertex2.y / mWorldScale);
		return 4;
	}

	int ExportedPhysics2DEdge::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetVertex)
			EXPORT_API(GetVertex)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics2DEdge, apis);
		SET_PARENT(L, Physics2DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics2DEdge, api_c);
		return 0;
	}

	ExportedPhysics2DEdge*ExportedPhysics2DEdge::New(){
		ExportedPhysics2DEdge*p2d = CreateExportedAliceType<ExportedPhysics2DEdge>();
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.userData = p2d;
		p2d->mBody = GetPhysicsManager().m2DPhysicWorld->CreateBody(&bodyDef);
		return p2d;
	}
}