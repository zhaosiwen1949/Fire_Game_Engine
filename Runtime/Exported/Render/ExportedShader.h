#pragma once
#include "Runtime/Render/Shader.h"
namespace Alice{
	class ExportedShader :public Shader {
	public:
		static int Export(lua_State*L);
	public:
		DEFINE_LUA_API(New);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedShader, Shader)
	};
}