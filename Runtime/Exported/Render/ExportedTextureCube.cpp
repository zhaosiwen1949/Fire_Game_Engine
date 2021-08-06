#include "ExportedTextureCube.h"

namespace Alice{
	int ExportedTextureCube::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, TextureCube, apis);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, TextureCube, api_c);
		return 0;
	}
}