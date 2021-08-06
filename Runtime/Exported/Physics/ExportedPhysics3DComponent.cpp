#include "ExportedPhysics3DComponent.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Math/AliceMatrix4x4.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	int ExportedPhysics3DComponent::API_SetLinearVelocity(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		((physx::PxRigidDynamic*)ptr->mRigidActor)->setLinearVelocity(physx::PxVec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)),false);
		return 0;
	}
	int ExportedPhysics3DComponent::API_GetLinearVelocity(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		//const b2Vec2 & v = ptr->mBody->GetLinearVelocity();
		//lua_pushnumber(L, v.x);
		//lua_pushnumber(L, v.y);
		return 0;
	}
	int ExportedPhysics3DComponent::API_EnableGravity(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		if (lua_toboolean(L,2)){
			ptr->mRigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		}
		else {
			ptr->mRigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
		}
		return 0;
	}
	int ExportedPhysics3DComponent::API_LockAngular(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		ptr->LockAngular(lua_toboolean(L, 2), lua_toboolean(L, 3), lua_toboolean(L, 4));
		return 0;
	}
	int ExportedPhysics3DComponent::API_LockVelocity(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		ptr->LockLinear(lua_toboolean(L, 2), lua_toboolean(L, 3), lua_toboolean(L, 4));
		return 0;
	}
	int ExportedPhysics3DComponent::API_SetOffset(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		ptr->mRigidOffset.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		return 0;
	}
	int ExportedPhysics3DComponent::API_Test(lua_State*L) {
		ExportedPhysics3DComponent*ptr = TO_USERDATA(L, ExportedPhysics3DComponent, 1);
		const Matrix4x4&world_matrix = ptr->mOwner->GetWorldMatrix();
		physx::PxVec3 position(world_matrix.mData[12],world_matrix.mData[13],world_matrix.mData[14]);
		physx::PxTransform transform(position);
		physx::PxFixedJoint*joint=physx::PxFixedJointCreate(*GetPhysicsManager().mPhysxEngine,
			ptr->mRigidActor, transform,
			nullptr, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0)));
		return 0;
	}
	int ExportedPhysics3DComponent::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetLinearVelocity)
			EXPORT_API(GetLinearVelocity)
			EXPORT_API(EnableGravity)
			EXPORT_API(LockVelocity)
			EXPORT_API(LockAngular)
			EXPORT_API(SetOffset)
			EXPORT_API(Test)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DComponent, apis);
		SET_PARENT(L, Component);
		return 0;
	}
}