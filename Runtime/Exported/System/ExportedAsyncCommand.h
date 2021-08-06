#pragma once
#include "Runtime/Thread/AliceThread.h"
namespace Alice{
	class ExportedAsyncCommand :public AsyncCommand {
	public:
		DEFINE_LUA_API(Execute);
		DEFINE_LUA_API(SetSrcFolder);
		DEFINE_LUA_API(SetDstFolder);
		DEFINE_LUA_API(AddCopyFile);
		DEFINE_LUA_API(AddDeleteFile);
		DEFINE_LUA_API(CopyFiles);
		DEFINE_LUA_API(DeleteFiles);
		DEFINE_LUA_API(Release);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedAsyncCommand, AsyncCommand)
	};
}
