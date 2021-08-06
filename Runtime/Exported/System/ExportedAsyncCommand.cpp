#include "ExportedAsyncCommand.h"
namespace Alice{
	//暂时不要调用Post接口，Post的方式比较复杂，所以基本还是由脚本层自己实现
	int ExportedAsyncCommand::API_Execute(lua_State*L){
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		const char * command = lua_tostring(L, 2);
		ptr->mCommand.Resize(strlen(command) + 1);
		ptr->mCommand = command;
		ptr->mAction = kAsyncCommandActionSystemCommand;
		ptr->AsyncInvoke();
		return 0;
	}
	int ExportedAsyncCommand::API_SetSrcFolder(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		const char * src_folder = lua_tostring(L, 2);
		ptr->mSrcFolder.Resize(strlen(src_folder) + 1);
		ptr->mSrcFolder = src_folder;
		return 0;
	}
	int ExportedAsyncCommand::API_SetDstFolder(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		const char * dst_folder = lua_tostring(L, 2);
		ptr->mDstFolder.Resize(strlen(dst_folder) + 1);
		ptr->mDstFolder = dst_folder;
		return 0;
	}
	int ExportedAsyncCommand::API_AddCopyFile(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		const char * src_file = lua_tostring(L, 2);
		const char * dst_file = lua_tostring(L, 3);
		ptr->mSrcs.push_back(src_file);
		ptr->mDsts.push_back(dst_file);
		return 0;
	}
	int ExportedAsyncCommand::API_AddDeleteFile(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		const char * src_file = lua_tostring(L, 2);
		ptr->mSrcs.push_back(src_file);
		return 0;
	}
	int ExportedAsyncCommand::API_CopyFiles(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		ptr->mAction = kAsyncCommandActionCopyFiles;
		ptr->AsyncInvoke();
		return 0;
	}
	int ExportedAsyncCommand::API_DeleteFiles(lua_State*L) {
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		ptr->mAction = kAsyncCommandActionDeleteFiles;
		ptr->AsyncInvoke();
		return 0;
	}
	int ExportedAsyncCommand::API_Release(lua_State*L){
		//这里删除的是指向物体的指针，会调用析构函数
		ExportedAsyncCommand*ptr = TO_USERDATA(L, ExportedAsyncCommand, 1);
		ptr->Destroy();
		return 0;
	}
	int ExportedAsyncCommand::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(Execute)
			EXPORT_API(SetSrcFolder)
			EXPORT_API(SetDstFolder)
			EXPORT_API(AddCopyFile)
			EXPORT_API(AddDeleteFile)
			EXPORT_API(CopyFiles)
			EXPORT_API(DeleteFiles)
			EXPORT_API(Release)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, AsyncCommand, apis);
		SET_PARENT(L, AliceThread);
		luaL_Reg api_c[] ={
			{"New",New_ExportedAliceType},
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, AsyncCommand, api_c);
		lua_pushnumber(L, kAsyncCommandActionSystemCommand);
		lua_setfield(L, -2, "ActionSystemCommand");
		lua_pushnumber(L, kAsyncCommandActionCopyFiles);
		lua_setfield(L, -2, "ActionCopyFiles");
		lua_pushnumber(L, kAsyncCommandActionDeleteFiles);
		lua_setfield(L, -2, "ActionDeleteFiles");
		return 0;
	}
}
