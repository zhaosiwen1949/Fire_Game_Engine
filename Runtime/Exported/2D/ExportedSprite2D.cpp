#include "ExportedSprite2D.h"
namespace Alice{
	int ExportedSprite2D::API_SetTexture(lua_State*L) {
		ExportedSprite2D*ptr = TO_USERDATA(L, ExportedSprite2D, 1);
		const char * image_path = lua_tostring(L, 2);
		ptr->SetTexture(image_path);
		return 0;
	}
	int ExportedSprite2D::Export(lua_State*L){
		luaL_Reg member_functions[] = {
			EXPORT_API(SetTexture)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Sprite2D, member_functions);

		SET_PARENT(L, Component2D);

		luaL_Reg package_functions[] = {
			EXPORT_API(ClassID) 
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Sprite2D, package_functions);
		return 0;
	}
}
