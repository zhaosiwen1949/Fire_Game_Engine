#include "ExportedPhysics3DTriangleMesh.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Exported/3D/ExportedMesh.h"
namespace Alice{
	int ExportedPhysics3DTriangleMesh::API_SetSize(lua_State*L){
		ExportedPhysics3DTriangleMesh*ptr = TO_USERDATA(L, ExportedPhysics3DTriangleMesh, 1);
		//ptr->SetSize((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		return 0;
	}
	int ExportedPhysics3DTriangleMesh::API_GetSize(lua_State*L){
		ExportedPhysics3DTriangleMesh*ptr = TO_USERDATA(L, ExportedPhysics3DTriangleMesh, 1);
		lua_pushnumber(L, 1.0f);
		lua_pushnumber(L, 1.0f);
		return 2;
	}
	int ExportedPhysics3DTriangleMesh::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DTriangleMesh, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DTriangleMesh, api_c);
		return 0;
	}
	ExportedPhysics3DTriangleMesh*ExportedPhysics3DTriangleMesh::New(void*data) {
		ExportedMesh*mesh = TO_USERDATA(LuaEngine::s_GlobalStatePtr, ExportedMesh, 3);
		ExportedPhysics3DTriangleMesh*p3d = CreateExportedAliceType<ExportedPhysics3DTriangleMesh>();
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = mesh->mVertexData->mCount;
		meshDesc.points.stride = sizeof(VertexDataFull);
		meshDesc.points.data = mesh->mVertexData->mData;

		//meshDesc.triangles.count = mesh->mIndexBuffer.mSize;
		//meshDesc.triangles.stride = 3 * sizeof(AliceUInt16);
		//meshDesc.triangles.data = mesh->mIndexBuffer.mIndexes;

		meshDesc.flags.set(physx::PxMeshFlag::e16_BIT_INDICES);

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		bool status = GetPhysicsManager().mPhysxEngineCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
		if (!status)
			return NULL;

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh*triangle_mesh=GetPhysicsManager().mPhysxEngine->createTriangleMesh(readBuffer);

		physx::PxRigidStatic* rigid = GetPhysicsManager().mPhysxEngine->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));

		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*rigid,physx::PxTriangleMeshGeometry(triangle_mesh), *mDefaultMaterial);
		if (!shape)
			Error("Physics3DTriangleMesh::New create shape failed!");
		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = shape;
		return p3d;
	}
}