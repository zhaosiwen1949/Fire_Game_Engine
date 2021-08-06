#include "ExportedRay.h"
namespace Alice{
	int ExportedRay::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(GetOrigin)
			EXPORT_API(GetDirection)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Ray, apis);
		luaL_Reg api_c[] = 
		{ 
			EXPORT_API(ClassID)
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Ray, api_c);
		return 0;
	}
	int ExportedRay::API_GetOrigin(lua_State*L){
		ExportedRay*ptr = TO_USERDATA(L, ExportedRay, 1);
		lua_pushnumber(L, ptr->mOrigin.x);
		lua_pushnumber(L, ptr->mOrigin.y);
		lua_pushnumber(L, ptr->mOrigin.z);
		return 3;
	}
	int ExportedRay::API_GetDirection(lua_State*L){
		ExportedRay*ptr = TO_USERDATA(L, ExportedRay, 1);
		lua_pushnumber(L, ptr->mDirection.x);
		lua_pushnumber(L, ptr->mDirection.y);
		lua_pushnumber(L, ptr->mDirection.z);
		return 3;
	}
}