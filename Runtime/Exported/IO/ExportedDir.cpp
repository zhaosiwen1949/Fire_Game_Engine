#include "ExportedDir.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ResourceManager.h"
namespace Alice{
	int ExportedDir::API_Create(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		if (!ResourceManager::Exist(path)) {
			lua_pushboolean(L, ResourceManager::CreateDir(path));
		}else {
			lua_pushboolean(L, false);
		}
		return 1;
	}
	int ExportedDir::API_Delete(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		if (ResourceManager::Exist(path)) {
			lua_pushboolean(L,ResourceManager::DeleteDir(path));
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}
	int ExportedDir::API_GetItemList(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		FileItemNode itemNode;
		lua_newtable(L);
		ResourceManager::GetItemListInDirectory(L,path);
		return 1;
	}

	int ExportedDir::API_Exist(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		if (ResourceManager::Exist(path)) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int ExportedDir::Export(lua_State*L){
		luaL_Reg api_c[] =
		{
			EXPORT_API(Exist)
			EXPORT_API(Create)
			EXPORT_API(Delete)
			EXPORT_API(GetItemList)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Dir, api_c);
		return 0;
	}
}
