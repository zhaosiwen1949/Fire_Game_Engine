#include "ExportedString.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/IO/IOSBundleManager.h"
namespace Alice{
	int ExportedString::API_UTF8ToASCII(lua_State*L){
#if ALICE_PLATFORM_WIN
        const char *src = lua_tostring(L,1);
		TCHAR szBuffer[1024] = {0};
		StringUtils::UTF8ToUnicode(src, szBuffer);
		char dst[1024] = {0};
		StringUtils::UnicodeToASCII(szBuffer, dst, 1024);
		lua_pushstring(L, dst);
#endif
		return 1;
	}
	int ExportedString::API_ASCIIToUTF8(lua_State*L) {
#if ALICE_PLATFORM_WIN
		const char *src = lua_tostring(L, 1);
		TCHAR szBuffer[1024] = { 0 };
		StringUtils::ASCIIToUnicode(src, szBuffer);
		char dst[1024] = { 0 };
		StringUtils::UnicodeToUTF8(szBuffer, dst);
		lua_pushstring(L, dst);
#endif
		return 1;
	}
	int ExportedString::API_UnicodeToASCII(lua_State*L) {
#if ALICE_PLATFORM_WIN
		const char *src = lua_tostring(L, 1);
		char dst[1024] = { 0 };
		StringUtils::UnicodeToASCII((LPTSTR)src, dst, 1024);
		lua_pushstring(L, dst);
#endif
		return 1;
	}
	int ExportedString::API_UnicodeToUTF8(lua_State*L) {
#if ALICE_PLATFORM_WIN
		const char *src = lua_tostring(L, 1);
		char dst[1024] = { 0 };
		StringUtils::UnicodeToUTF8((LPTSTR)src, dst);
		lua_pushstring(L, dst);
#endif
		return 1;
	}
	int ExportedString::API_ASCIIToUnicode(lua_State*L) {
#if ALICE_PLATFORM_WIN
		const char *src = lua_tostring(L, 1);
		TCHAR szBuffer[1024] = { 0 };
		StringUtils::ASCIIToUnicode(src, szBuffer);
		lua_pushstring(L, (char*)szBuffer);
#endif
		return 1;
	}
	int ExportedString::API_UTF8ToUnicode(lua_State*L) {
#if ALICE_PLATFORM_WIN
		const char *src = lua_tostring(L, 1);
		TCHAR szBuffer[1024] = { 0 };
		StringUtils::UTF8ToUnicode(src, szBuffer);
		lua_pushstring(L, (char*)szBuffer);
#endif
		return 1;
	}

	int ExportedString::Export(lua_State*L){
		luaL_Reg api_c[] ={
			EXPORT_API(UTF8ToASCII)
			EXPORT_API(ASCIIToUTF8)
			EXPORT_API(ASCIIToUnicode)
			EXPORT_API(UTF8ToUnicode)
			EXPORT_API(UnicodeToASCII)
			EXPORT_API(UnicodeToUTF8)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, String, api_c);
		return 0;
	}
}
