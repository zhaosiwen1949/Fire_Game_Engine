#include "ExportedPhysics3DSphere.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Debugger/Log.h"
namespace Alice{
	int ExportedPhysics3DSphere::API_SetOffset(lua_State*L){
		ExportedPhysics3DSphere*ptr = TO_USERDATA(L, ExportedPhysics3DSphere, 1);
		ptr->SetOffset(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		return 0;
	}
	int ExportedPhysics3DSphere::API_SetRadius(lua_State*L) {
		ExportedPhysics3DSphere*ptr = TO_USERDATA(L, ExportedPhysics3DSphere, 1);
		ptr->SetRadius(lua_tonumber(L, 2));
		return 0;
	}
	int ExportedPhysics3DSphere::API_GetPosition(lua_State*L) {
		ExportedPhysics3DSphere*ptr = TO_USERDATA(L, ExportedPhysics3DSphere, 1);
		physx::PxTransform transform=ptr->mRigidActor->getGlobalPose();
		lua_pushnumber(L, transform.p.x);
		lua_pushnumber(L, transform.p.y);
		lua_pushnumber(L, transform.p.z);
		return 3;
	}
	int ExportedPhysics3DSphere::API_GetRadius(lua_State*L) {
		ExportedPhysics3DSphere*ptr = TO_USERDATA(L, ExportedPhysics3DSphere, 1);
		physx::PxSphereGeometry sphere;
		ptr->mShape->getSphereGeometry(sphere);
		lua_pushnumber(L, sphere.radius);
		return 1;
	}
	int ExportedPhysics3DSphere::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetOffset)
			EXPORT_API(SetRadius)
			EXPORT_API(GetPosition)
			EXPORT_API(GetRadius)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DSphere, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DSphere, api_c);
		return 0;
	}
	ExportedPhysics3DSphere*ExportedPhysics3DSphere::New(){
		ExportedPhysics3DSphere*p3d = CreateExportedAliceType<ExportedPhysics3DSphere>();
		physx::PxRigidDynamic* rigid = GetPhysicsManager().mPhysxEngine->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0)));
		physx::PxShape* shape = rigid->createShape(physx::PxSphereGeometry(0.5f), *mDefaultMaterial);
		if (!shape)
			Error("Physics3DSphere::New create shape failed!");
		physx::PxRigidBodyExt::updateMassAndInertia(*rigid, 1.0f);
		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = shape;
		return p3d;
	}
}