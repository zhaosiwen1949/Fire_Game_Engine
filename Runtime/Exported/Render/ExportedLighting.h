#pragma once
#include "Runtime/Render/light/Light.h"

namespace Alice{
	class ExportedLight :public Light{
	public:
		DEFINE_LUA_API(SetPosition);
		DEFINE_LUA_API(SetAmbient);
		DEFINE_LUA_API(SetDiffuse);
		DEFINE_LUA_API(SetSpecular);
		DEFINE_LUA_API(SetConstant);//for spot light and point light
		DEFINE_LUA_API(SetLinear);//for spot light and point light
		DEFINE_LUA_API(SetQuadric);//for spot light and point light
		DEFINE_LUA_API(SetCutoff);//for spot light
		DEFINE_LUA_API(SetExponent);//for spot light
		DEFINE_LUA_API(SetDirection);//for spot light
		DEFINE_LUA_API(SetShiness);//
		DEFINE_LUA_API(SetSetting);
		DEFINE_LUA_API(SetSetting1);
		DEFINE_LUA_API(SetCulling);
		DEFINE_LUA_API(SetProjection);
		DEFINE_LUA_API(SetOrtho);
		DEFINE_LUA_API(LookAt);
		DEFINE_LUA_API(SetType);
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(New);
		DEFINE_LUA_API(Delete);
		DEFINE_LUA_API(Clear);
		DEFINE_LUA_API(SetMainLight);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedLight, Light)
	};
}
