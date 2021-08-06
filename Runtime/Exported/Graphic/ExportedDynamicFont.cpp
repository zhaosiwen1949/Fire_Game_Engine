#include "ExportedDynamicFont.h"
namespace Alice{
	int ExportedDynamicFont::API_SetFont(lua_State * L) {
		ExportedDynamicFont*ptr = TO_USERDATA(L, ExportedDynamicFont, 1);
		const char * font_path = lua_tostring(L, 2);
		Alice::FontResource*fontData = Alice::Resource::LoadResource<Alice::FontResource>(font_path);
		if (fontData != nullptr) {
			FontFace*face = new (kMemFontId)FontFace;
			if (face->Init(fontData)) {
				face->mFontData = fontData;
				ptr->mFace = face;
				lua_pushboolean(L, true);
			}
			else {
				delete fontData;
				delete face;
				lua_pushboolean(L, false);
			}
		}
		return 1;
	}
	int ExportedDynamicFont::API_SetFontData(lua_State * L) {
		ExportedDynamicFont*ptr = TO_USERDATA(L, ExportedDynamicFont, 1);
		const char * font_data = lua_tostring(L, 2);
		int data_len = lua_tointeger(L, 3);
		FontFace*face = new (kMemFontId)FontFace;
		if (face->InitWithFontFileData((unsigned char*)font_data,data_len)) {
			ptr->mFace = face;
			lua_pushboolean(L, true);
		}
		else {
			delete face;
			lua_pushboolean(L, false);
		}
		return 1;
	}
	int ExportedDynamicFont::API_Release(lua_State * L) {
		ExportedDynamicFont*ptr = TO_USERDATA(L, ExportedDynamicFont, 1);
		delete ptr;
		return 0;
	}
	int ExportedDynamicFont::API_GetGlobalHeight(lua_State * L) {
		ExportedDynamicFont*ptr = TO_USERDATA(L, ExportedDynamicFont, 1);
		lua_pushnumber(L, ptr->mFace->GetLineHeight(lua_tointeger(L,2)));
		return 1;
	}
	int ExportedDynamicFont::Export(lua_State*L) {
		luaL_Reg apis[] = {
			EXPORT_API(SetFont)
			EXPORT_API(SetFontData)
			EXPORT_API(GetGlobalHeight)
			EXPORT_API(Release)
			EXPORT_API(ClassID)
		{
			NULL,NULL
		}
		};
		NEW_LUA_TYPE_WITH_API(L, DynamicFont, apis);
		luaL_Reg api_c[] = {
			{"New",New_ExportedAliceType},
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, DynamicFont, api_c);
		return 0;
	}
}