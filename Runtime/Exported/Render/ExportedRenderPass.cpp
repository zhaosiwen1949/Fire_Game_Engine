#include "ExportedRenderPass.h"
#include "ExportedTexture2D.h"
#include "ExportedTextureCube.h"
#include "ExportedMaterial.h"
#include "Runtime/Exported/Math/ExportedMatrix3x3.h"
#include "Runtime/Exported/Math/ExportedMatrix4x4.h"
namespace Alice {
	int ExportedRenderPass::API_EnableZTest(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbZTest = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_EnableZWrite(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbWriteZ = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_ZTest(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mZTestFunction = lua_tointeger(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_Offset(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mOffsetFactor = lua_tonumber(L, 2);
		ptr->mRenderState.mOffsetUnit = lua_tonumber(L, 3);
		return 0;
	}
	int ExportedRenderPass::API_EnableCull(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbCull = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_Cull(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mCullFace = lua_tointeger(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_EnableBlend(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbBlend = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_BlendFunc(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mSRCBlendFunc = lua_tointeger(L, 2);
		ptr->mRenderState.mDSTBlendFunc = lua_tointeger(L, 3);
		return 0;
	}
	int ExportedRenderPass::API_EnableAlphaTest(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbAlphaTest = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_AlphaTest(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mAlphaTestFunction = lua_tointeger(L, 2);
		ptr->mRenderState.mAlphaTestValue = lua_tonumber(L, 3);
		return 0;
	}
	int ExportedRenderPass::API_EnablePointSprite(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mRenderState.mbEnablePointSprite=lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_WriteStencilMask(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mbWriteStencilMask=lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_UseStencilMask(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mbUseStencilMask = lua_toboolean(L, 2);
		return 0;
	}
	int ExportedRenderPass::API_SetTexture2D(lua_State*L) {
		if (lua_isstring(L, 3)) {
			const char*propName = lua_tostring(L, 2);
			const char*textureGUID = lua_tostring(L, 3);
			Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
			if (texture != nullptr) {
				ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
				ptr->SetTextureProperty(propName, texture);
			}
		}
		else if (lua_isuserdata(L, 3)) {
			ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
			const char*propName = lua_tostring(L, 2);
			ExportedTexture2D*texture = TO_USERDATA(L, ExportedTexture2D, 3);
			ptr->SetTextureProperty(propName, texture);
		}
		return 0;
	}
	int ExportedRenderPass::API_GetTexture2D(lua_State*L) {
		const char*uniformName = lua_tostring(L, 2);
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		TextureUnitProperty*texture = ptr->GetTextureProperty(uniformName);
		if (texture != nullptr&&texture->mValue != nullptr) {
			texture->mValue->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
	int ExportedRenderPass::API_SetTextureCube(lua_State*L){
		if (lua_gettop(L) != 3) {
			errorC("Material:SetTextureCube need 3 arg");
			return 0;
		}
		if (lua_isstring(L, 3)) {
			const char*propName = lua_tostring(L, 2);
			const char*texturePath = lua_tostring(L, 3);
			TextureCube*textureCube = TextureCube::LoadTextureCube(texturePath);
			if (textureCube != nullptr) {
				CreateLuaDataViaExistObject(textureCube, TextureCube, L);
				ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
				ptr->SetTextureCubeProperty(propName, textureCube);
			}
		}
		else if (lua_isuserdata(L, 3)) {
			ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
			const char*propName = lua_tostring(L, 2);
			ExportedTextureCube*cubeMap = TO_USERDATA(L, ExportedTextureCube, 3);
			ptr->SetTextureCubeProperty(propName, cubeMap);
		}
		return 0;
	}
	int ExportedRenderPass::API_GetTextureCube(lua_State*L) {
		const char*uniformName = lua_tostring(L, 2);
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		TextureUnitProperty*texture = ptr->GetTextureProperty(uniformName);
		if (texture != nullptr&&texture->mValue != nullptr) {
			texture->mValue->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
	int ExportedRenderPass::API_SetFloat(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->SetFloatProperty(lua_tostring(L, 2), lua_tonumber(L, 3));
		return 0; 
	}
	int ExportedRenderPass::API_SetVec2(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->SetVec2Property(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		return 0;
	}
	int ExportedRenderPass::API_SetVec3(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->SetVec3Property(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedRenderPass::API_SetVec4(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->SetVec4Property(lua_tostring(L, 2), lua_tonumber(L,3),lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6));
		return 0;
	}
	int ExportedRenderPass::API_SetMat3(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ExportedMatrix3x3*mat3 = TO_USERDATA(L, ExportedMatrix3x3, 3);
		ptr->SetMatrix3Property(lua_tostring(L, 2), mat3->mData);
		return 0;
	}
	int ExportedRenderPass::API_SetMat4(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ExportedMatrix4x4*mat4 = TO_USERDATA(L, ExportedMatrix4x4, 3);
		ptr->SetMatrix4Property(lua_tostring(L, 2), mat4->mData);
		return 0;
	}
	int ExportedRenderPass::API_SetType(lua_State*L) {
		ExportedRenderPass*ptr = TO_USERDATA(L, ExportedRenderPass, 1);
		ptr->mCatagory = (RenderPassCatagory)lua_tointeger(L, 2);
		return 0;
	}
	int ExportedRenderPass::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetType)
			EXPORT_API(EnableAlphaTest)
			EXPORT_API(AlphaTest)
			EXPORT_API(EnablePointSprite)
			EXPORT_API(EnableZTest)
			EXPORT_API(EnableZWrite)
			EXPORT_API(ZTest)
			EXPORT_API(Offset)
			EXPORT_API(EnableCull)
			EXPORT_API(Cull)
			EXPORT_API(EnableBlend)
			EXPORT_API(BlendFunc)
			EXPORT_API(WriteStencilMask)
			EXPORT_API(UseStencilMask)
			EXPORT_API(SetTexture2D)
			EXPORT_API(GetTexture2D)
			EXPORT_API(SetTextureCube)
			EXPORT_API(GetTextureCube)
			EXPORT_API(SetFloat)
			EXPORT_API(SetVec2)
			EXPORT_API(SetVec3)
			EXPORT_API(SetVec4)
			EXPORT_API(SetMat3)
			EXPORT_API(SetMat4)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, RenderPass, apis);
		luaL_Reg api_c[] = { 
			EXPORT_API(New)
			EXPORT_API(ClassID) 
			EXPORT_API_END 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, RenderPass, api_c);
		lua_pushnumber(L, kRenderPassCatagoryNoLit);
		lua_setfield(L, -2, "NoLit");
		lua_pushnumber(L, kRenderPassCatagoryOnePass);
		lua_setfield(L, -2, "OnePass");
		lua_pushnumber(L, kRenderPassCatagoryForwardBase);
		lua_setfield(L, -2, "ForwardBase");
		lua_pushnumber(L, kRenderPassCatagoryForwardAdd);
		lua_setfield(L, -2, "ForwardAdd");
		lua_pushnumber(L, kRenderPassCatagoryShadowCaster);
		lua_setfield(L, -2, "ShadowCaster");
		lua_pushnumber(L, kRenderPassCatagoryProjector);
		lua_setfield(L, -2, "Projector");
		return 0;
	}
	int ExportedRenderPass::API_New(lua_State*L) {
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
			RenderPass * pass = Alice::CreateExportedAliceType<ExportedRenderPass>();
			pass->SetShader(shader);
			pass->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
}