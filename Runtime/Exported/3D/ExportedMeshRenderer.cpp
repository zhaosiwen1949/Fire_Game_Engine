#include "ExportedMeshRenderer.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
namespace Alice{
	int ExportedMeshRenderer::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetMaterialCount)
			EXPORT_API(SetMaterial)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, MeshRenderer, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = 
		{ 
			EXPORT_API(ClassID)
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, MeshRenderer, api_c);
		return 0;
	}
	int ExportedMeshRenderer::API_SetMaterialCount(lua_State*L)
	{
		ExportedMeshRenderer*ptr = TO_USERDATA(L, ExportedMeshRenderer, -2);
		ptr->SetMaterialCount(lua_tointeger(L,-1));
		return 0;
	}
	int ExportedMeshRenderer::API_SetMaterial(lua_State*L)
	{
		ExportedMeshRenderer*ptr = TO_USERDATA(L, ExportedMeshRenderer, -3);
		ExportedMaterial*mat= TO_USERDATA(L, ExportedMaterial, -1);
		ptr->SetMaterial(lua_tointeger(L,-2), mat);
		return 1;
	}
}