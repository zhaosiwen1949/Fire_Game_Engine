#include "ExportedPhysics3DCube.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Debugger/Log.h"
namespace Alice{
	int ExportedPhysics3DCube::API_SetSize(lua_State*L){
		ExportedPhysics3DCube*ptr = TO_USERDATA(L, ExportedPhysics3DCube, 1);
		ptr->SetSize((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4));
		return 0;
	}
	int ExportedPhysics3DCube::API_GetSize(lua_State*L){
		ExportedPhysics3DCube*ptr = TO_USERDATA(L, ExportedPhysics3DCube, 1);
		physx::PxBoxGeometry box;
		ptr->mShape->getBoxGeometry(box);
		lua_pushnumber(L, box.halfExtents.x*2.0f);
		lua_pushnumber(L, box.halfExtents.y*2.0f);
		lua_pushnumber(L, box.halfExtents.z*2.0f);
		return 3;
	}
	int ExportedPhysics3DCube::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DCube, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DCube, api_c);
		return 0;
	}
	ExportedPhysics3DCube*ExportedPhysics3DCube::New(){
		ExportedPhysics3DCube*p3d = CreateExportedAliceType<ExportedPhysics3DCube>();
		physx::PxRigidDynamic* rigid = GetPhysicsManager().mPhysxEngine->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));
		physx::PxShape* box = rigid->createShape(physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), *mDefaultMaterial);
		if (!box)
			Error("Physics3DCube::New create shape failed!");
		physx::PxRigidBodyExt::updateMassAndInertia(*rigid,1.0f);
		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = box;
		return p3d;
	}
}