#pragma once
#include "ResourceManager.h"

namespace Alice
{
    class IOSBundleManager
    {
    public:
        static bool LoadInternalData(const char*path,Data&data);
        static const char* GetOuterDataPath();
        static const char* GetDesktopPath();
        static void OpenURL(const char*url);
        static void GetItemListInDirectory(lua_State*L, const char * path);
        static bool DeleteFileWithPath(const char * path);
        static bool DeleteDir(const char*path);
    };
}
