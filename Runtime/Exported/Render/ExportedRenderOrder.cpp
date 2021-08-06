#include "ExportedRenderOrder.h"
namespace Alice{
	int ExportedRenderOrder::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, RenderOrder, apis);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) EXPORT_API_END };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, RenderOrder, api_c);
		return 0;
	}
}