#include "ExportedPhysics2DCircle.h"
#include "Runtime/Physics/PhysicsManager.h"

namespace Alice
{
	int ExportedPhysics2DCircle::API_SetRadius(lua_State*L)
	{
		ExportedPhysics2DCircle*ptr = TO_USERDATA(L, ExportedPhysics2DCircle, 1);
		((b2CircleShape*)ptr->mFixture->GetShape())->m_radius = lua_tonumber(L, 2)*mWorldScale;
		return 0;
	}

	int ExportedPhysics2DCircle::API_GetRadius(lua_State*L){
		ExportedPhysics2DCircle*ptr = TO_USERDATA(L, ExportedPhysics2DCircle, 1);
		lua_pushnumber(L,((b2CircleShape*)ptr->mFixture->GetShape())->m_radius / mWorldScale);
		return 1;
	}

	int ExportedPhysics2DCircle::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetRadius)
			EXPORT_API(GetRadius)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics2DCircle, apis);
		SET_PARENT(L, Physics2DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics2DCircle, api_c);
		return 0;
	}

	ExportedPhysics2DCircle*ExportedPhysics2DCircle::New(){
		ExportedPhysics2DCircle*p2d = CreateExportedAliceType<ExportedPhysics2DCircle>();
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.userData = p2d;
		p2d->mBody = GetPhysicsManager().m2DPhysicWorld->CreateBody(&bodyDef);
		b2CircleShape circle;
		circle.m_radius = 0.5f;
		p2d->mFixture=p2d->mBody->CreateFixture(&circle,1.0f);
		return p2d;
	}
}