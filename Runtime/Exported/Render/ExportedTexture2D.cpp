#include "ExportedTexture2D.h"
#include "Runtime/Debugger/Debugger.h"
namespace Alice{
	int ExportedTexture2D::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(Init)
			EXPORT_API(Parameteri)
			EXPORT_API(InitMipmap)
			EXPORT_API(Release)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Texture2D, apis);
		luaL_Reg api_c[] = { 
			EXPORT_API(New)
			EXPORT_API(ClassID) 
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Texture2D, api_c);
		lua_pushnumber(L, GL_TEXTURE_MIN_FILTER);
		lua_setfield(L, -2, "MIN_FILTER");
		lua_pushnumber(L, GL_TEXTURE_MAG_FILTER);
		lua_setfield(L, -2, "MAG_FILTER");
		lua_pushnumber(L, GL_TEXTURE_WRAP_S);
		lua_setfield(L, -2, "WRAP_S");
		lua_pushnumber(L, GL_TEXTURE_WRAP_T);
		lua_setfield(L, -2, "WRAP_T");
#if ALICE_IPHONE || ALICE_ANDROID
#else
		lua_pushnumber(L, GL_TEXTURE_WRAP_R);
		lua_setfield(L, -2, "WRAP_R");
#endif
		lua_pushnumber(L, GL_LINEAR);
		lua_setfield(L, -2, "LINEAR");
		lua_pushnumber(L, GL_NEAREST);
		lua_setfield(L, -2, "NEAREST");
		lua_pushnumber(L, GL_CLAMP_TO_EDGE);
		lua_setfield(L, -2, "CLAMP_TO_EDGE");
		lua_pushnumber(L, GL_REPEAT);
		lua_setfield(L, -2, "REPEAT");
		return 0;
	}
	int ExportedTexture2D::API_New(lua_State*L) {
		ExportedTexture2D::New_ExportedAliceType(L);
		Texture2D*ptr = TO_USERDATA(L, Texture2D, 1);
		ptr->retain();
		return 1;
	}
	int ExportedTexture2D::API_Release(lua_State*L) {
		Texture2D*ptr = TO_USERDATA(L, Texture2D, 1);
		ptr->release();
		return 0;
	}
	int ExportedTexture2D::API_Init(lua_State*L) {
		Texture2D*ptr = TO_USERDATA(L, Texture2D, 1);
		ptr->InitTexture2D(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}
	int ExportedTexture2D::API_Parameteri(lua_State*L) {
		Texture2D*ptr = TO_USERDATA(L, Texture2D, 1);
		ptr->TexParameteri(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}
	int ExportedTexture2D::API_InitMipmap(lua_State*L) {
		Texture2D*ptr = TO_USERDATA(L, Texture2D, 1);
		OGL_CALL(glBindTexture(GL_TEXTURE_2D, ptr->mTextureID));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		OGL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
		return 0;
	}
}
