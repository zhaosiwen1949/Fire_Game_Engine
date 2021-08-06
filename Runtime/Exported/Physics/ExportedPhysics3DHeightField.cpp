#include "ExportedPhysics3DHeightField.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Exported/IO/ExportedImageResource.h"
#include "Runtime/Exported/3D/ExportedTerrain.h"
namespace Alice{
	int ExportedPhysics3DHeightField::API_SetSize(lua_State*L){
		ExportedPhysics3DHeightField*ptr = TO_USERDATA(L, ExportedPhysics3DHeightField, 1);
		//ptr->SetSize((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		return 0;
	}
	int ExportedPhysics3DHeightField::API_GetSize(lua_State*L){
		ExportedPhysics3DHeightField*ptr = TO_USERDATA(L, ExportedPhysics3DHeightField, 1);
		lua_pushnumber(L, 1.0f);
		lua_pushnumber(L, 1.0f);
		return 2;
	}
	int ExportedPhysics3DHeightField::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Physics3DHeightField, apis);
		SET_PARENT(L, Physics3DComponent);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Physics3DHeightField, api_c);
		return 0;
	}
	ExportedPhysics3DHeightField*ExportedPhysics3DHeightField::New(void*data) {
		ExportedTerrain*terrain = TO_USERDATA(LuaEngine::s_GlobalStatePtr, ExportedTerrain, 3);
		int sample_count = terrain->mRes->mWidth*terrain->mRes->mHeight;
		ExportedPhysics3DHeightField*p3d = CreateExportedAliceType<ExportedPhysics3DHeightField>();
		physx::PxHeightFieldSample* samples = new physx::PxHeightFieldSample[sample_count];
		for (int z = 0; z < terrain->mRes->mHeight; ++z) {
			for (int x = 0; x < terrain->mRes->mWidth; ++x) {
				int sample_index = terrain->mRes->mWidth*z + x;
				int offset_y = terrain->mRes->mHeight - z - 1;
				int image_data_index = offset_y * terrain->mRes->mWidth + x;
				samples[sample_index].height = terrain->mRes->mPixelData.mData[image_data_index];
			}
		}
		physx::PxHeightFieldDesc hfDesc;
		hfDesc.format = physx::PxHeightFieldFormat::eS16_TM;
		hfDesc.nbColumns = terrain->mRes->mWidth;
		hfDesc.nbRows = terrain->mRes->mHeight;
		hfDesc.samples.data = samples;
		hfDesc.samples.stride = sizeof(physx::PxHeightFieldSample);
		hfDesc.flags.set(physx::PxHeightFieldFlag::eNO_BOUNDARY_EDGES);

		physx::PxHeightField* aHeightField = GetPhysicsManager().mPhysxEngineCooking->createHeightField(hfDesc, GetPhysicsManager().mPhysxEngine->getPhysicsInsertionCallback());

		physx::PxRigidStatic* rigid = GetPhysicsManager().mPhysxEngine->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));

		physx::PxHeightFieldGeometry hfGeom(aHeightField, physx::PxMeshGeometryFlags(), 1, 1, 1);
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*rigid, hfGeom, &mDefaultMaterial, 1);

		GetPhysicsManager().mPhysxScence->addActor(*rigid);
		rigid->userData = p3d;
		p3d->mRigidActor = rigid;
		p3d->mShape = shape;
		return p3d;
	}
}