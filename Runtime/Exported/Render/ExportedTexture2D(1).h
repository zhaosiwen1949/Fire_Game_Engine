#pragma once
#include "Runtime/Render/GL20/Texture2D.h"
namespace Alice{
	class ExportedTexture2D :public Texture2D{
	public:
		DEFINE_LUA_API(Init);
		DEFINE_LUA_API(Parameteri);
		DEFINE_LUA_API(InitMipmap);
		DEFINE_LUA_API(Release);
		DEFINE_LUA_API(New);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedTexture2D, Texture2D)
	};
}
