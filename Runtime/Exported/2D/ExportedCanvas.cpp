#include "ExportedCanvas.h"
namespace Alice{
	int ExportedCanvas::Export(lua_State*L){
		luaL_Reg member_functions[] = {
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Canvas, member_functions);
		SET_PARENT(L, Component2D);
		luaL_Reg package_functions[] = {
			EXPORT_API(ClassID) 
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Canvas, package_functions);
		return 0;
	}
}
