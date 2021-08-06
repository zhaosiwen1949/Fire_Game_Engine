#pragma once
#include "Runtime/RuntimePrefix.h"
#if ALICE_ANDROID
#include "AliceData.h"
#include "Runtime/Plugins/Lua/lua.hpp"
namespace Alice{
	class AndroidAssetReader{
	public:
		static bool LoadInternalData(const char*path,Data &data);
		static void GetItemListInDirectory(lua_State*L, const char * path);
		static bool DeleteDir(const char *path);
		static bool DeleteFileWithPath(const char *path);
		static AAssetManager*sAssetManager;
	};
}
#endif