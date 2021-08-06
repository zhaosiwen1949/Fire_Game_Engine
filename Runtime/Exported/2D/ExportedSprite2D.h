#pragma once
#include "Runtime/2D/Sprite2D.h"
namespace Alice{
	class ExportedSprite2D : public Sprite2D {
	public:
		DEFINE_LUA_API(SetTexture);
		static int Export(lua_State*L);
	public:
		//c++ class¡¢lua 
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedSprite2D, Sprite2D)
	};
}
