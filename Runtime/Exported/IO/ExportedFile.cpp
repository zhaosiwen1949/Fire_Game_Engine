#include "ExportedFile.h"
#include "Runtime/IO/ResourceManager.h"
namespace Alice{
	int ExportedFile::API_Exist(lua_State*L){
		const char * path = lua_tostring(L, 1);
		if (ResourceManager::Exist(path)) {
			lua_pushboolean(L, true);
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}
	int ExportedFile::API_Delete(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		if (ResourceManager::DeleteFileWithPath(path)) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int ExportedFile::Export(lua_State*L){
		luaL_Reg api_c[] ={
			EXPORT_API(Exist)
			EXPORT_API(Delete)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, File, api_c);
		return 0;
	}
}