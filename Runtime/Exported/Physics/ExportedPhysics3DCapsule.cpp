#include "ExportedPhysics3DCapsule.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Debugger/Log.h"
namespace Alice{
	int ExportedPhysics3DCapsule::API_SetInfo(lua_State*L){
		ExportedPhysics3DCapsule*ptr = TO_USERDATA(L, ExportedPhysics3DCapsule, 1);
		ptr->SetInfo(lua_tonumber(L, 2), lua_tonumber(L, 3));
		return 0;
	}
	int ExportedPhysics3DCapsule::API_GetInfo(lua_State*L){
		ExportedPhysics3DCapsule*ptr = TO_USERDATA(L, ExportedPhysics3DCapsule, 1);
		physx::PxCapsuleGeometry capsule;
		ptr->mShape->getCapsuleGeometry(capsule);
		lua_pushnumber(L, capsule.radius);
		lua_pushnumber(L, capsule.halfHeight);
		return 2;
	}
	int ExportedPhysics3DCapsule::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetInfo)
			EXPORT_API(GetInfo)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DCapsule, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DCapsule, api_c);
		return 0;
	}
	ExportedPhysics3DCapsule*ExportedPhysics3DCapsule::New(){
		ExportedPhysics3DCapsule*p3d = CreateExportedAliceType<ExportedPhysics3DCapsule>();
		physx::PxRigidDynamic* rigid = GetPhysicsManager().mPhysxEngine->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
		physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1.0f)));
		physx::PxShape* box = rigid->createShape(physx::PxCapsuleGeometry(0.5f,0.2f), *mDefaultMaterial);
		box->setLocalPose(relativePose);
		if (!box)
			Error("Physics3DCapsule::New create shape failed!");
		physx::PxRigidBodyExt::updateMassAndInertia(*rigid,1.0f);
		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = box;
		return p3d;
	}
}