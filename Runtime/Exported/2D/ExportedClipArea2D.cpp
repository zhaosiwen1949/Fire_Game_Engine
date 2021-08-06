#include "ExportedClipArea2D.h"
namespace Alice{
	int ExportedClipArea2D::API_SetSize(lua_State*L) {
		ExportedClipArea2D*ptr = TO_USERDATA(L, ExportedClipArea2D, -3);
		ptr->SetSize(lua_tonumber(L, -2), lua_tonumber(L, -1));
		return 0;
	}
	int ExportedClipArea2D::Export(lua_State*L){
		luaL_Reg member_functions[] = {
			EXPORT_API(SetSize)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, ClipArea2D, member_functions);

		SET_PARENT(L, Component2D);

		luaL_Reg package_functions[] = {
			EXPORT_API(ClassID) 
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, ClipArea2D, package_functions);
		return 0;
	}
}
