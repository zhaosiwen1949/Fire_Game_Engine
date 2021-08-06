
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/Exported/IO/ExportedImageResource.h"
#include "ExportedTerrain.h"
namespace Alice{
	int ExportedTerrain::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetHeightMap)
			EXPORT_API(SetMaterial)
			EXPORT_API(SetHeightScale)
			EXPORT_API(SetXZScale)
			EXPORT_API(GetInfo)
			EXPORT_API(GetHeight)
			EXPORT_API(GetNormal)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Terrain, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = { 
			EXPORT_API(ClassID)
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Terrain, api_c);
		return 0;
	}
	int ExportedTerrain::API_SetHeightMap(lua_State*L){
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		ImageResource*image_resource = ImageResource::Init(lua_tostring(L, 2));
		if (image_resource != nullptr) {
			ptr->InitWidthHeightData(image_resource->mPixelData.mData, image_resource->mWidth, image_resource->mHeight);
			ptr->mRes = image_resource;
		}
		return 0;
	}
	int ExportedTerrain::API_SetMaterial(lua_State*L){
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		ptr->SetMaterial(TO_USERDATA(L, ExportedMaterial, 2));
		return 0;
	}
	int ExportedTerrain::API_SetHeightScale(lua_State*L) {
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		ptr->SetXYZScale(ptr->mXValueScale, lua_tonumber(L, 2), ptr->mZValueScale);
		return 0;
	}
	int ExportedTerrain::API_SetXZScale(lua_State*L) {
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		ptr->SetXYZScale(lua_tonumber(L, 2), ptr->mYValueScale, lua_tonumber(L, 3));
		return 0;
	}
	int ExportedTerrain::API_GetHeight(lua_State*L) {
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		lua_pushnumber(L, ptr->GetHeight(lua_tonumber(L, 2), lua_tonumber(L, 3)));
		return 1;
	}
	int ExportedTerrain::API_GetInfo(lua_State*L) {
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		Vector3f n;
		float h;
		ptr->GetInfo(lua_tonumber(L, 2), lua_tonumber(L, 3),n,h);
		lua_pushnumber(L, n.x);
		lua_pushnumber(L, n.y);
		lua_pushnumber(L, n.z);
		lua_pushnumber(L, h);
		return 4;
	}
	int ExportedTerrain::API_GetNormal(lua_State*L) {
		ExportedTerrain*ptr = TO_USERDATA(L, ExportedTerrain, 1);
		Vector3f n;
		ptr->GetNormal(lua_tonumber(L, 2), lua_tonumber(L, 3), n);
		lua_pushnumber(L, n.x);
		lua_pushnumber(L, n.y);
		lua_pushnumber(L, n.z);
		return 3;
	}
}