#include "ExportedMatrix4x4.h"
namespace Alice{
	int ExportedMatrix4x4::API_Invert(lua_State*L) {
		Matrix4x4*ptr = TO_USERDATA(L, Matrix4x4, 1);
		float result[16];
		InvertMatrix4x4(ptr->mData, result);
		memcpy(ptr->mData, result, sizeof(float) * 16);
		return 0;
	}
	int ExportedMatrix4x4::Export(lua_State*L) {
		luaL_Reg apis[] = {
			EXPORT_API(Invert)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Matrix4x4, apis);
		luaL_Reg api_c[] ={
			{"New",New_ExportedAliceType},
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Matrix4x4, api_c);
		return 0;
	}
}