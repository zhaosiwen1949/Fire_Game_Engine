#include "ExportedPhysics3DConvexMesh.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Debugger/Log.h"
namespace Alice{
	int ExportedPhysics3DConvexMesh::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DConvexMesh, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DConvexMesh, api_c); 
		return 0;
	}
	ExportedPhysics3DConvexMesh*ExportedPhysics3DConvexMesh::New(){
		ExportedPhysics3DConvexMesh*p3d = CreateExportedAliceType<ExportedPhysics3DConvexMesh>();
		physx::PxRigidDynamic* rigid = GetPhysicsManager().mPhysxEngine->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));
		static const physx::PxVec3 convexVerts[] = { physx::PxVec3(0,1,0),physx::PxVec3(1,0,0),physx::PxVec3(-1,0,0),physx::PxVec3(0,0,1),physx::PxVec3(0,0,-1) };
		physx::PxConvexMeshDesc convexDesc;
		physx::PxConvexMesh* convexMesh = nullptr;
		convexDesc.points.count = 5;
		convexDesc.points.stride = sizeof(physx::PxVec3);
		convexDesc.points.data = convexVerts;
		bool use_standard_offline_cooking = false;
		if (use_standard_offline_cooking) {
			convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
			physx::PxDefaultMemoryOutputStream buf;
			physx::PxConvexMeshCookingResult::Enum result;
			if (!GetPhysicsManager().mPhysxEngineCooking->cookConvexMesh(convexDesc, buf, &result))
				return NULL;
			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			convexMesh = GetPhysicsManager().mPhysxEngine->createConvexMesh(input);
		}
		else {
			convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eDISABLE_MESH_VALIDATION | physx::PxConvexFlag::eFAST_INERTIA_COMPUTATION;
			convexMesh = GetPhysicsManager().mPhysxEngineCooking->createConvexMesh(convexDesc, GetPhysicsManager().mPhysxEngine->getPhysicsInsertionCallback());
		}
		physx::PxShape* convex = physx::PxRigidActorExt::createExclusiveShape(*rigid, physx::PxConvexMeshGeometry(convexMesh), *mDefaultMaterial);
		if (!convex)
			Error("Physics3DCube::New create shape failed!");
		physx::PxRigidBodyExt::updateMassAndInertia(*rigid,1.0f);
		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = convex;
		return p3d;
	}
}