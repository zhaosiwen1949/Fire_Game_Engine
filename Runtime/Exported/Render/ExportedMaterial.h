#pragma once
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/Material.h"
namespace Alice{
	class Shader;
	class ExportedCull {
	public:
		static int Export(lua_State*L);
	};
	class ExportedBlendFunc {
	public:
		static int Export(lua_State*L);
	};
	class ExportedZTest {
	public:
		static int Export(lua_State*L);
	};
	class ExportedMaterial : public Material{
	public:
		DEFINE_LUA_API(SetVec4);
		DEFINE_LUA_API(AddSpriteInfo);
		DEFINE_LUA_API(SetTexture2D);
		DEFINE_LUA_API(GetTexture2D);
		DEFINE_LUA_API(SetTextureCube);
		DEFINE_LUA_API(SetTexture2DDepthColorBuffer);
		DEFINE_LUA_API(DetectUniform);
		DEFINE_LUA_API(ZWrite);
		DEFINE_LUA_API(EnableZTest);
		DEFINE_LUA_API(ZTest);
		DEFINE_LUA_API(Offset);
		DEFINE_LUA_API(EnableCull);
		DEFINE_LUA_API(Cull);
		DEFINE_LUA_API(AddPass);
		DEFINE_LUA_API(RemovePass);
		DEFINE_LUA_API(SetPass);
		DEFINE_LUA_API(GetPass);
		DEFINE_LUA_API(EnableBlend);
		DEFINE_LUA_API(BlendFunc);
		DEFINE_LUA_API(EnableDepthTest);
		DEFINE_LUA_API(EnableClipDistance);
		DEFINE_LUA_API(EnableTwoSideShading);
		DEFINE_LUA_API(EnableWiredFrame);
		DEFINE_LUA_API(SetLightingPass);
		DEFINE_LUA_API(SetProcedureTexture2D);
		DEFINE_LUA_API(EnablePointSprite);
		DEFINE_LUA_API(SetPrimitiveType);
		DEFINE_LUA_API(EnableShadow);
		DEFINE_LUA_API(SetAmbientTexture);
		DEFINE_LUA_API(SetDiffuseTexture);
		DEFINE_LUA_API(SetSpecularTexture);
		DEFINE_LUA_API(SetNormalTexture);
		DEFINE_LUA_API(SetAlphaTexture);
		DEFINE_LUA_API(SetTextureMask);
		DEFINE_LUA_API(WriteStencilMask);
		DEFINE_LUA_API(UseStencilMask);
		DEFINE_LUA_API(SetOnePass);
		DEFINE_LUA_API(SetForwardBasePass);
		DEFINE_LUA_API(SetForwardAdditivePass);
		DEFINE_LUA_API(SetRenderQueue);
		DEFINE_LUA_API(GetRenderQueue);
	public:
		static int Export(lua_State*L);
		static Material*Clone(const char * material);
		static Shader* CreateShaderFromRuntimeAsset(const char *shader);
		static Shader* CreateShaderFromFile(const char *shader);
		static void NewMaterialWithOnPassShader(lua_State*L);
		static void NewEmptyMaterial(lua_State*L);
		DEFINE_LUA_API(New);
		DEFINE_LUA_API(Clone);
		DEFINE_LUA_API(Get);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedMaterial, Material)
	};
}
