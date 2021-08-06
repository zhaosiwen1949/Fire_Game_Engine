#include "ExportedPhysics2DBox.h"
#include "Runtime/Physics/PhysicsManager.h"

namespace Alice
{
	int ExportedPhysics2DBox::API_SetSize(lua_State*L)
	{
		ExportedPhysics2DBox*ptr = TO_USERDATA(L, ExportedPhysics2DBox, 1);
		ptr->SetSize((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		return 0;
	}

	int ExportedPhysics2DBox::API_GetSize(lua_State*L)
	{
		ExportedPhysics2DBox*ptr = TO_USERDATA(L, ExportedPhysics2DBox, 1);
		lua_pushnumber(L, ptr->mSize.x);
		lua_pushnumber(L, ptr->mSize.y);
		return 2;
	}

	int ExportedPhysics2DBox::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics2DBox, apis);
		SET_PARENT(L, Physics2DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics2DBox, api_c);
		return 0;
	}

	ExportedPhysics2DBox*ExportedPhysics2DBox::New()
	{
		ExportedPhysics2DBox*p2d = CreateExportedAliceType<ExportedPhysics2DBox>();
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.type = b2_dynamicBody;
		bodyDef.userData = p2d;
		p2d->mBody = GetPhysicsManager().m2DPhysicWorld->CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(0.5f, 0.5f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		p2d->mFixture = p2d->mBody->CreateFixture(&fixtureDef);
		return p2d;
	}
}