#pragma once
#include "Runtime/2D/ImageSprite.h"

namespace Alice{
	class ExportedImageSprite:public ImageSprite{
	public:
		DEFINE_LUA_API(SetTexture);
		DEFINE_LUA_API(SetImageData);
		DEFINE_LUA_API(SetRGBAData);
		DEFINE_LUA_API(SetRGBData);
		DEFINE_LUA_API(SetQRCodeData);
		DEFINE_LUA_API(GetImagePath);
		DEFINE_LUA_API(SetCameraColorBuffer);
		DEFINE_LUA_API(SetCameraDepthColorBuffer);
		DEFINE_LUA_API(SetSprite);
		DEFINE_LUA_API(SetMaterial);
		DEFINE_LUA_API(GetMaterial);
		DEFINE_LUA_API(SetAlpha);
		DEFINE_LUA_API(SetColor);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
		//index,x,y,z
		DEFINE_LUA_API(UpdateNormal);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTangent);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTexcoord1);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedImageSprite, ImageSprite)
	};
}
