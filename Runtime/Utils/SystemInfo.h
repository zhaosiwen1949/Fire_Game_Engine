#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/String/FixedString.h"
enum RuntimePlatformType{
	RuntimePlatformTypeWindowsPlayer = 1,
	RuntimePlatformTypeWindowsEditor,
	RuntimePlatformTypeiPhonePlayer,
	RuntimePlatformTypeAndroidPlayer,
	RuntimePlatformTypeRuntimePlatformCount
};
enum SystemLanguage {
	SystemLanguageChinese,
	SystemLanguageEnglish,
};
namespace Alice {
	class SystemInfo{
	public:
		static int RuntimePlatform(lua_State*L);
		static int OpenURL(lua_State*L);
		static void OpenURL(const char*url);
	};
}