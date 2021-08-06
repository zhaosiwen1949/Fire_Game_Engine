#pragma once
#include "Runtime/Physics/Physics2DComponent.h"

namespace Alice
{
	class ExportedPhysics2DComponent :public Physics2DComponent
	{
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetDensity);
		DEFINE_LUA_API(SetFriction);
		DEFINE_LUA_API(SetRestitution);
		DEFINE_LUA_API(SetLinearVelocity);
		DEFINE_LUA_API(SetAngularVelocity);
		DEFINE_LUA_API(SetLinearDamping);
		DEFINE_LUA_API(SetAngularDamping);
		DEFINE_LUA_API(SetAllowSleep);
		DEFINE_LUA_API(SetAwake);
		DEFINE_LUA_API(SetFixedRotation);
		DEFINE_LUA_API(SetType);
		DEFINE_LUA_API(SetActive);
		DEFINE_LUA_API(SetGravityScale);
		DEFINE_LUA_API(SetBullet);
		DEFINE_LUA_API(SetCatagory);
		DEFINE_LUA_API(SetMask);
		DEFINE_LUA_API(GetMask);
		DEFINE_LUA_API(GetCatagory);
		DEFINE_LUA_API(GetDensity);
		DEFINE_LUA_API(GetFriction);
		DEFINE_LUA_API(GetRestitution);
		DEFINE_LUA_API(GetLinearVelocity);
		DEFINE_LUA_API(GetAngularVelocity);
		DEFINE_LUA_API(GetLinearDamping);
		DEFINE_LUA_API(GetAngularDamping);
		DEFINE_LUA_API(GetAllowSleep);
		DEFINE_LUA_API(GetAwake);
		DEFINE_LUA_API(GetFixedRotation);
		DEFINE_LUA_API(GetType);
		DEFINE_LUA_API(GetActive);
		DEFINE_LUA_API(GetGravityScale);
		DEFINE_LUA_API(GetBullet);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics2DComponent, Physics2DComponent)
	};
}
