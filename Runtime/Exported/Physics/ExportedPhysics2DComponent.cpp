#include "ExportedPhysics2DComponent.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Debugger/Log.h"

namespace Alice
{
	int ExportedPhysics2DComponent::API_SetDensity(lua_State*L){
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mFixture->SetDensity((float)lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetDensity(lua_State*L){
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mFixture->GetDensity());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetFriction(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		float friction = (float)lua_tonumber(L, 2);
		ptr->mFixture->SetFriction(friction);
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetFriction(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mFixture->GetFriction());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetRestitution(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mFixture->SetRestitution((float)lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetRestitution(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mFixture->GetRestitution());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetLinearVelocity(lua_State*L) {
		if (!lua_isnumber(L, 2)||!(lua_isnumber(L,3))) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		b2Vec2 v(lua_tonumber(L,2),lua_tonumber(L,3));
		ptr->mBody->SetLinearVelocity (v);
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetLinearVelocity(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		const b2Vec2 & v = ptr->mBody->GetLinearVelocity();
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		return 2;
	}

	int ExportedPhysics2DComponent::API_SetAngularVelocity(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetAngularVelocity(lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetAngularVelocity(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mBody->GetAngularVelocity());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetLinearDamping(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetLinearDamping(lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetLinearDamping(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mBody->GetLinearDamping());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetAngularDamping(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetAngularDamping(lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetAngularDamping(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mBody->GetAngularDamping());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetAllowSleep(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetSleepingAllowed(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetAllowSleep(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushboolean(L, ptr->mBody->IsSleepingAllowed());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetAwake(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetAwake(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetAwake(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushboolean(L, ptr->mBody->IsAwake());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetFixedRotation(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetFixedRotation(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetFixedRotation(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushboolean(L, ptr->mBody->IsFixedRotation());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetType(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetType((b2BodyType)lua_tointeger(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetType(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushinteger(L, ptr->mBody->GetType());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetActive(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetActive(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetActive(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushboolean(L, ptr->mBody->IsActive());
		return 1;
	}

	int ExportedPhysics2DComponent::API_SetBullet(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetBullet(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetBullet(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushboolean(L, ptr->mBody->IsBullet());
		return 1;
	}
	int ExportedPhysics2DComponent::API_SetGravityScale(lua_State*L) {
		if (!lua_isnumber(L, 2)) {
			errorC("ExportedPhysics2DComponent:SetFriction arg at index 2 must be a number,current is 2(%s)", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->mBody->SetGravityScale(lua_tonumber(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetGravityScale(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->mBody->GetGravityScale());
		return 1;
	}
	int ExportedPhysics2DComponent::API_SetCatagory(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->SetCatagoryBits(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetCatagory(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->GetCatagoryBits());
		return 1;
	}
	int ExportedPhysics2DComponent::API_SetMask(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		ptr->SetMaskBits(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedPhysics2DComponent::API_GetMask(lua_State*L) {
		ExportedPhysics2DComponent*ptr = TO_USERDATA(L, ExportedPhysics2DComponent, 1);
		lua_pushnumber(L, ptr->GetMaskBits());
		return 1;
	}
	int ExportedPhysics2DComponent::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetDensity)
			EXPORT_API(SetFriction)
			EXPORT_API(SetRestitution)
			EXPORT_API(SetLinearVelocity)
			EXPORT_API(SetAngularVelocity)
			EXPORT_API(SetLinearDamping)
			EXPORT_API(SetAngularDamping)
			EXPORT_API(SetAllowSleep)
			EXPORT_API(SetAwake)
			EXPORT_API(SetFixedRotation)
			EXPORT_API(SetType)
			EXPORT_API(SetActive)
			EXPORT_API(SetGravityScale)
			EXPORT_API(SetBullet)
			EXPORT_API(SetCatagory)
			EXPORT_API(SetMask)
			EXPORT_API(GetDensity)
			EXPORT_API(GetFriction)
			EXPORT_API(GetRestitution)
			EXPORT_API(GetLinearVelocity)
			EXPORT_API(GetAngularVelocity)
			EXPORT_API(GetLinearDamping)
			EXPORT_API(GetAngularDamping)
			EXPORT_API(GetAllowSleep)
			EXPORT_API(GetAwake)
			EXPORT_API(GetFixedRotation)
			EXPORT_API(GetType)
			EXPORT_API(GetActive)
			EXPORT_API(GetGravityScale)
			EXPORT_API(GetBullet)
			EXPORT_API(GetCatagory)
			EXPORT_API(GetMask)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics2DComponent, apis);
		SET_PARENT(L, Component);
		return 0;
	}
}