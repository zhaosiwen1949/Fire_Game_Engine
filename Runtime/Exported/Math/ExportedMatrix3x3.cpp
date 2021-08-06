#include "ExportedMatrix3x3.h"
namespace Alice{
	int ExportedMatrix3x3::API_Invert(lua_State*L) {
		Matrix3x3*ptr = TO_USERDATA(L, Matrix3x3, 1);
		ptr->Invert();
		return 0;
	}
	int ExportedMatrix3x3::Export(lua_State*L) {
		luaL_Reg apis[] = {
			EXPORT_API(Invert)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Matrix3x3, apis);
		luaL_Reg api_c[] ={
			{"New",New_ExportedAliceType},
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Matrix3x3, api_c);
		return 0;
	}
}