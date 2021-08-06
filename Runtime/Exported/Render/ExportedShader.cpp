#include "ExportedShader.h"
#include "ExportedMaterial.h"
namespace Alice{
	int ExportedShader::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Shader, apis);
		luaL_Reg api_c[] = {
			EXPORT_API(New)
			EXPORT_API(ClassID)
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Shader, api_c);
		return 0;
	}
	int ExportedShader::API_New(lua_State*L) {
		const char*shader_user_path = lua_tostring(L, 1);
		Shader * shader = Shader::GetCached(shader_user_path);
		if (shader == nullptr) {
#if _DEBUG || DEV_PUBLISH
			shader = ExportedMaterial::CreateShaderFromFile(shader_user_path);
#else
			shader = ExportedMaterial::CreateShaderFromRuntimeAsset(shader_user_path);
#endif
		}
		if (shader != nullptr) {
			shader->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
}