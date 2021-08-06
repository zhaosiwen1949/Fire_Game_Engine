#pragma once
#include "Runtime/2D/Sprite2D9.h"

namespace Alice
{
	class ExportedSprite2D9 :public Sprite2D9
	{
	public:
		DEFINE_LUA_API(SetTexture);
		DEFINE_LUA_API(SetSprite);
		DEFINE_LUA_API(SetMaterial);
		DEFINE_LUA_API(GetMaterial);
		DEFINE_LUA_API(SetAlpha);
		DEFINE_LUA_API(SetColor);
		DEFINE_LUA_API(SetAdditionalColor);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
		DEFINE_LUA_API(SetCenterRect);
		DEFINE_LUA_API(GetCenterRect);
		//index,x,y,z
		DEFINE_LUA_API(UpdateNormal);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTangent);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTexcoord1);
		DEFINE_LUA_API(BlendFunc);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedSprite2D9, Sprite2D9)
	};
}
