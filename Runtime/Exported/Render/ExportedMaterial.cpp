#include "ExportedMaterial.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Serializer/GLSL.serializer.h"
#include "Runtime/IO/ShaderResource.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "ExportedCamera.h"
#include "ExportedTextureCube.h"
#include "ExportedTexture2D.h"
#include "ExportedRenderPass.h"
#include "ExportedShader.h"

namespace Alice
{
	int ExportedMaterial::API_SetVec4(lua_State*L)
	{
		if (lua_gettop(L)!=6){
			errorC("Material:SetVec4 need 4 args");
			return 0;
		}
		if (!lua_isstring(L, 2)){
			errorC("Material:SetVec4 arg 1 is not a string");
			return 0;
		}
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		mat->SetVec4Property(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}

	int ExportedMaterial::API_SetTexture2D(lua_State*L){
		if (lua_gettop(L) != 3){
			errorC("Material:SetTexture2D need 3 arg");
			return 0;
		}
		if (lua_isstring(L,3)){
			const char*propName = lua_tostring(L, 2);
			const char*textureGUID = lua_tostring(L, 3);
			Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
			if (texture!=nullptr){
				ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
				mat->SetTextureProperty(propName, texture);
			}
		}else if (lua_isuserdata(L, 3)) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			const char*propName = lua_tostring(L, 2);
			ExportedTexture2D*texture = TO_USERDATA(L, ExportedTexture2D, 3);
			mat->SetTextureProperty(propName, texture);
		}
		return 0;
	}

	int ExportedMaterial::API_GetTexture2D(lua_State*L){
		if (!lua_isstring(L,2)){
			lua_pushnil(L);
			return 1;
		}
		const char*uniformName = lua_tostring(L, 2);
		Material*mat = TO_USERDATA(L, ExportedMaterial, 1);
		TextureUnitProperty*texture = mat->GetTextureProperty(uniformName);
		if (texture != nullptr&&texture->mValue != nullptr) {
			texture->mValue->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedMaterial::API_SetTextureCube(lua_State*L)
	{
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
				ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
				mat->SetTextureCubeProperty(propName, textureCube);
			}
		}
		else if (lua_isuserdata(L, 3)) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			const char*propName = lua_tostring(L, 2);
			ExportedTextureCube*cubeMap = TO_USERDATA(L, ExportedTextureCube, 3);
			mat->SetTextureCubeProperty(propName, cubeMap);
		}
		return 0;
	}
	int ExportedMaterial::API_SetTexture2DDepthColorBuffer(lua_State*L)
	{
		if (lua_gettop(L) != 3) {
			errorC("Material:SetTexture2DDepthColorBuffer need 3 arg");
			return 0;
		}
		const char*propName = lua_tostring(L, 2);
		ExportedCamera*camera = TO_USERDATA(L, ExportedCamera, 3);
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		mat->SetTextureProperty(propName, camera->mDepthFBO->mColorBuffer.mPtr);
		return 0;
	}

	int ExportedMaterial::API_SetProcedureTexture2D(lua_State*L){
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		const char*prop_name = lua_tostring(L, 2);
		int size = lua_tointeger(L, 3);
		char texture_name[256] = { 0 };
		sprintf(texture_name, "AlphaGradiant%d", size);
		Texture2D*texture = Texture2D::GetCachedTexture2D(texture_name);
		if (texture==nullptr){
			AliceAny rgba_data = GenAlphaGradiantQuadImage(size);
			texture = new Texture2D;
			texture->InitTexture2D(size, size);
			texture->UploadTexture(rgba_data);
			texture->mName = texture_name;
			Texture2D::mTextures.insert(std::pair<std::string, Texture2D*>(texture_name, texture));
			delete[] (AliceUInt8*)rgba_data;
		}
		mat->SetTextureProperty(prop_name, texture);
		return 0;
	}

	int ExportedMaterial::API_EnablePointSprite(lua_State*L){
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->EnablePointSprite(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedMaterial::API_DetectUniform(lua_State*L){
		return 0;
	}

	int ExportedMaterial::API_EnableBlend(lua_State*L)
	{
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		if (lua_isnumber(L,2)){
			ptr->EnableBlend(lua_tointeger(L, 2) == 0 ? false : true);
		}else if (lua_isboolean(L,2)){
			ptr->EnableBlend(lua_toboolean(L, 2));
		}
		return 0;
	}

	int ExportedMaterial::API_BlendFunc(lua_State*L){
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->BlendFunc(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}

	int ExportedMaterial::API_EnableDepthTest(lua_State*L){
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->EnableZTest(lua_toboolean(L, 2));
		return 0;
	}
	int ExportedMaterial::API_EnableZTest(lua_State*L){
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->EnableZTest(lua_toboolean(L, 2));
		return 0;
	}
	int ExportedMaterial::API_ZWrite(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->EnableZWrite(lua_toboolean(L, 2));
		return 0;
	}
	int ExportedMaterial::API_ZTest(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->ZTestFunction(lua_tointeger(L, 2));
		return 0;
	}
	int ExportedMaterial::API_Offset(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->ZOffset(lua_tonumber(L, 2), lua_tonumber(L, 3));
		return 0;
	}
	int ExportedMaterial::API_EnableCull(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->EnableCull(lua_toboolean(L, 2));
		return 0;
	}
	int ExportedMaterial::API_Cull(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->SetCullFace(lua_tointeger(L, 2));
		return 0;
	}
	int ExportedMaterial::API_SetPass(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPassCatagory catagory = (RenderPassCatagory)lua_tointeger(L, 2);
		RenderPass*pass = TO_USERDATA(L, RenderPass, 3);
		pass->mCatagory = catagory;
		ptr->AddPass(pass);
		return 0;
	}
	int ExportedMaterial::API_AddPass(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPass*pass = TO_USERDATA(L, RenderPass, 2);
		ptr->AddPass(pass);
		return 0;
	}
	int ExportedMaterial::API_RemovePass(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->RemovePass(lua_tointeger(L, 2));
		return 0;
	}
	int ExportedMaterial::API_GetPass(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPass*pass = ptr->GetPass(lua_tointeger(L, 2));
		if (pass!=nullptr){
			pass->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
	int ExportedMaterial::API_AddSpriteInfo(lua_State*L){
		return 0;
	}
	int ExportedMaterial::API_EnableClipDistance(lua_State*L)
	{
		//ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
		glEnable(GL_CLIP_DISTANCE0);
#endif
		return 0;
	}

	int ExportedMaterial::API_EnableTwoSideShading(lua_State*L){//old api,may be used
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		if (lua_toboolean(L,2)){
			ptr->EnableCull(false);
		}
		else {
			ptr->EnableCull(true);
		}
		return 0;
	}

	int ExportedMaterial::API_EnableWiredFrame(lua_State*L){
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
		GLenum mode = GL_LINE;
		if (lua_isnumber(L, 2)) {
			mode = lua_tointeger(L, 2) == 1 ? GL_LINE : GL_FILL;
		}
		mat->SetPolygonMode(mode);
#endif
		return 0;
	}

	int ExportedMaterial::API_SetLightingPass(lua_State*L) {
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		if (lua_tointeger(L, 2) == 2) {
			mat->GetPass(0)->mCatagory = kRenderPassCatagoryOnePass;
		}
		return 0;
	}

	int ExportedMaterial::API_SetPrimitiveType(lua_State*L){
		if (!lua_isuserdata(L, 1) ||
			!lua_isnumber(L, 2)){
			errorC("Material:SetPrimitiveType arg must be number");
			return 0;
		}
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->SetPrimitive(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedMaterial::API_EnableShadow(lua_State*L)
	{
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->mbReceiveShadow = lua_toboolean(L, 2);
		UniformVector4Property*prop = ptr->GetVec4Property(UniformDefines::Vec4_FragMixSetting_NameInShader);
		if (prop != nullptr) {
			prop->mValue.y = lua_toboolean(L, 2) ? 1.0f : 0.0f;
		}
		return 0;
	}

	int ExportedMaterial::API_SetAmbientTexture(lua_State*L)
	{
		const char*textureGUID = lua_tostring(L, 2);
		Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
		if (texture != nullptr) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			mat->SetTextureProperty(UniformDefines::Sampler_AmbientTexture_NameInShader, texture);
			UniformVector4Property*prop = mat->GetVec4Property(UniformDefines::Vec4_MaterialMask_NameInShader);
			if (prop != nullptr) {
				prop->mValue.x = 1.0f;
			}
		}
		return 0;
	}
	int ExportedMaterial::API_SetDiffuseTexture(lua_State*L)
	{
		const char*textureGUID = lua_tostring(L, 2);
		Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
		if (texture != nullptr) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			mat->SetTextureProperty(UniformDefines::Sampler_DiffuseTexture_NameInShader, texture);
			UniformVector4Property*prop = mat->GetVec4Property(UniformDefines::Vec4_MaterialMask_NameInShader);
			if (prop != nullptr) {
				prop->mValue.y = 1.0f;
			}
		}
		return 0;
	}
	int ExportedMaterial::API_SetSpecularTexture(lua_State*L)
	{
		const char*textureGUID = lua_tostring(L, 2);
		Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
		if (texture != nullptr) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			mat->SetTextureProperty(UniformDefines::Sampler_SpecularTexture_NameInShader, texture);
			UniformVector4Property*prop = mat->GetVec4Property(UniformDefines::Vec4_MaterialMask_NameInShader);
			if (prop != nullptr) {
				prop->mValue.z = 1.0f;
			}
		}
		return 0;
	}
	int ExportedMaterial::API_SetAlphaTexture(lua_State*L)
	{
		const char*textureGUID = lua_tostring(L, 2);
		Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
		if (texture != nullptr) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			mat->SetTextureProperty(UniformDefines::Sampler_AlphaTexture_NameInShader, texture);
			UniformVector4Property*prop = mat->GetVec4Property(UniformDefines::Vec4_MaterialMask_NameInShader);
			if (prop != nullptr) {
				prop->mValue.w = 1.0f;
			}
		}
		return 0;
	}
	int ExportedMaterial::API_SetNormalTexture(lua_State*L){
		const char*textureGUID = lua_tostring(L, 2);
		Texture2D*texture = Texture2D::LoadTexture2D(textureGUID);
		if (texture != nullptr) {
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
			mat->SetTextureProperty(UniformDefines::Sampler_NormalTexture_NameInShader, texture);
			UniformVector4Property*prop = mat->GetVec4Property(UniformDefines::Vec4_LightFlag_NameInShader);
			if (prop != nullptr) {
				prop->mValue.y = 1.0f;
			}
		}
		return 0;
	}

	int ExportedMaterial::API_SetTextureMask(lua_State*L)
	{
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		UniformVector4Property*mm = mat->GetVec4Property(UniformDefines::Vec4_MaterialMask_NameInShader);
		UniformVector4Property*lf = mat->GetVec4Property(UniformDefines::Vec4_LightFlag_NameInShader);
		if (mm != nullptr) {
			mm->mValue.x = lua_toboolean(L, 2) ? 1.0f : 0.0f;
			mm->mValue.y = lua_toboolean(L, 3) ? 1.0f : 0.0f;
			mm->mValue.z = lua_toboolean(L, 4) ? 1.0f : 0.0f;
			mm->mValue.w = lua_toboolean(L, 5) ? 1.0f : 0.0f;
		}
		if (lf!=nullptr){
			lf->mValue.y = lua_toboolean(L, 6) ? 1.0f : 0.0f;
		}
		return 0;
	}

	int ExportedMaterial::API_WriteStencilMask(lua_State*L){
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		mat->EnableWriteStencilMask(lua_toboolean(L, 2));
		return 0;
	}

	int ExportedMaterial::API_UseStencilMask(lua_State*L){
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		mat->EnableUseStencilMask(lua_toboolean(L, 2));
		return 0;
	}
	int ExportedMaterial::API_SetOnePass(lua_State*L) {
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPass*pass = TO_USERDATA(L, RenderPass, 2);
		pass->mCatagory = kRenderPassCatagoryOnePass;
		mat->AddPass(pass);
		return 0;
	}
	int ExportedMaterial::API_SetForwardBasePass(lua_State*L) {
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPass*pass = TO_USERDATA(L, RenderPass, 2);
		pass->mCatagory = kRenderPassCatagoryForwardBase;
		mat->AddPass(pass);
		return 0;
	}
	int ExportedMaterial::API_SetForwardAdditivePass(lua_State*L) {
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 1);
		RenderPass*pass = TO_USERDATA(L, RenderPass, 2);
		pass->mCatagory = kRenderPassCatagoryForwardAdd;
		mat->AddPass(pass);
		return 0;
	}
	int ExportedMaterial::API_SetRenderQueue(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		ptr->mRenderingQueue = (RenderingQueue)lua_tointeger(L, 2);
		return 0;
	}
	int ExportedMaterial::API_GetRenderQueue(lua_State*L) {
		ExportedMaterial*ptr = TO_USERDATA(L, ExportedMaterial, 1);
		lua_pushinteger(L, ptr->mRenderingQueue);
		return 1;
	}

	int ExportedCull::Export(lua_State*L) {
		luaL_Reg api_c[] = {
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Cull, api_c);
		lua_pushnumber(L, GL_FRONT);
		lua_setfield(L, -2, "FRONT");
		lua_pushnumber(L, GL_BACK);
		lua_setfield(L, -2, "BACK");
		lua_pushnumber(L, GL_FRONT_AND_BACK);
		lua_setfield(L, -2, "FRONT_AND_BACK");
		return 0;
	}
	int ExportedBlendFunc::Export(lua_State*L) {
		luaL_Reg api_c[] = {
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, BlendFunc, api_c);
		lua_pushnumber(L, GL_ONE);
		lua_setfield(L, -2, "ONE");
		lua_pushnumber(L, GL_ZERO);
		lua_setfield(L, -2, "ZERO");
		lua_pushnumber(L, GL_SRC_ALPHA);
		lua_setfield(L, -2, "SRC_ALPHA");
		lua_pushnumber(L, GL_SRC_COLOR);
		lua_setfield(L, -2, "SRC_COLOR");
		lua_pushnumber(L, GL_ONE_MINUS_SRC_ALPHA);
		lua_setfield(L, -2, "ONE_MINUS_SRC_ALPHA");
		lua_pushnumber(L, GL_ONE_MINUS_SRC_COLOR);
		lua_setfield(L, -2, "ONE_MINUS_SRC_COLOR");
		lua_pushnumber(L, GL_DST_ALPHA);
		lua_setfield(L, -2, "DST_ALPHA");
		lua_pushnumber(L, GL_DST_COLOR);
		lua_setfield(L, -2, "DST_COLOR");
		lua_pushnumber(L, GL_ONE_MINUS_DST_ALPHA);
		lua_setfield(L, -2, "ONE_MINUS_DST_ALPHA");
		lua_pushnumber(L, GL_ONE_MINUS_DST_COLOR);
		lua_setfield(L, -2, "ONE_MINUS_DST_COLOR");
		return 0;
	}
	int ExportedZTest::Export(lua_State*L) {
		luaL_Reg api_c[] = {
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, ZTest, api_c);
		lua_pushnumber(L, GL_LESS);
		lua_setfield(L, -2, "LESS");
		lua_pushnumber(L, GL_GREATER);
		lua_setfield(L, -2, "GREATER");
		lua_pushnumber(L, GL_LEQUAL);
		lua_setfield(L, -2, "LEQUAL");
		lua_pushnumber(L, GL_GEQUAL);
		lua_setfield(L, -2, "GEQUAL");
		lua_pushnumber(L, GL_EQUAL);
		lua_setfield(L, -2, "EQUAL");
		lua_pushnumber(L, GL_NOTEQUAL);
		lua_setfield(L, -2, "NOTEQUAL");
		lua_pushnumber(L, GL_ALWAYS);
		lua_setfield(L, -2, "ALWAYS");
		return 0;
	}
	int ExportedMaterial::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetVec4)
			EXPORT_API(AddSpriteInfo)
			EXPORT_API(SetTexture2D)
			EXPORT_API(GetTexture2D)
			EXPORT_API(SetTextureCube)
			EXPORT_API(SetTexture2DDepthColorBuffer)
			EXPORT_API(SetProcedureTexture2D)
			EXPORT_API(DetectUniform)
			EXPORT_API(EnableDepthTest)
			EXPORT_API(EnableZTest)
			EXPORT_API(ZWrite)
			EXPORT_API(ZTest)
			EXPORT_API(Offset)
			EXPORT_API(EnableCull)
			EXPORT_API(Cull)
			EXPORT_API(AddPass)
			EXPORT_API(RemovePass)
			EXPORT_API(SetPass)
			EXPORT_API(GetPass)
			EXPORT_API(EnableBlend)
			EXPORT_API(BlendFunc)
			EXPORT_API(EnableClipDistance)
			EXPORT_API(EnablePointSprite)
			EXPORT_API(EnableTwoSideShading)
			EXPORT_API(EnableWiredFrame)
			EXPORT_API(SetLightingPass)
			EXPORT_API(SetPrimitiveType)
			EXPORT_API(EnableShadow)
			EXPORT_API(SetAmbientTexture)
			EXPORT_API(SetDiffuseTexture)
			EXPORT_API(SetSpecularTexture)
			EXPORT_API(SetNormalTexture)
			EXPORT_API(SetAlphaTexture)
			EXPORT_API(SetTextureMask)
			EXPORT_API(WriteStencilMask)
			EXPORT_API(UseStencilMask)
			EXPORT_API(SetOnePass)
			EXPORT_API(SetForwardBasePass)
			EXPORT_API(SetForwardAdditivePass)
			EXPORT_API(SetRenderQueue)
			EXPORT_API(GetRenderQueue)
			EXPORT_API(ClassID)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Material, apis);
		luaL_Reg api_c[] = { 
			EXPORT_API(ClassID) 
			EXPORT_API(New)
			EXPORT_API(Get)
			EXPORT_API(Clone)
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Material, api_c);
		lua_pushnumber(L, kLightingPassOnePass);
		lua_setfield(L, -2, "LightingPassOnePass");
		lua_pushnumber(L, kLightingPassForwardBase);
		lua_setfield(L, -2, "LightingPassForwardBase");
		lua_pushnumber(L, kLightingPassForwardAdditive);
		lua_setfield(L, -2, "LightingPassForwardAdditive");
		lua_pushnumber(L, kLightingPassDeffered);
		lua_setfield(L, -2, "LightingPassDeffered");
		return 0;
	}
	Material*ExportedMaterial::Clone(const char * material) {
		auto iter = mCachedMaterials.find(material);
		if (iter != mCachedMaterials.end()){
			Material*ret = CreateExportedAliceType<ExportedMaterial>();
			*ret = *iter->second;
			ret->mbSharedMaterial = false;
			return ret;
		}
		return ExportedMaterial::mDefault2DMaterial;
	}
	int ExportedMaterial::API_Clone(lua_State*L){
		const char * material_name = lua_tostring(L, -1);
		auto iter = mCachedMaterials.find(material_name);
		if (iter!=mCachedMaterials.end()){
			Material*ret = CreateExportedAliceType<ExportedMaterial>();
			*ret = *iter->second;
			ret->mbSharedMaterial = false;
			ret->PushUserData(L);
		}else {
			auto iter = mCachedMaterials.find(material_name);
			if (iter != mCachedMaterials.end()) {
				Material*ret = CreateExportedAliceType<ExportedMaterial>();
				*ret = *iter->second;
				ret->mbSharedMaterial = false;
				ret->PushUserData(L);
			}
			else {
				Error("clone material failed %s",material_name);
				lua_pushnil(L);
			}
		}
		return 1;
	}
	int ExportedMaterial::API_Get(lua_State*L) {
		const char * material_name = lua_tostring(L, -1);
		auto iter = mCachedMaterials.find(material_name);
		if (iter != mCachedMaterials.end()) {
			iter->second->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}
	Shader* ExportedMaterial::CreateShaderFromFile(const char *shader_user_path) {
		FixedString vsFile(128), fsFile(128);
		vsFile = shader_user_path;
		vsFile << ".vs";
		fsFile = shader_user_path;
		fsFile << ".fs";
		Alice::Data vsData, fsData;
		if (Alice::ResourceManager::LoadData(vsFile.mText, vsData) &&
			Alice::ResourceManager::LoadData(fsFile.mText, fsData)){
			return Shader::CreateStandard(shader_user_path, (char*)vsData.mData, (char*)fsData.mData);
		} else {
			Error("CreateShaderFromFile fail,cannot load shader %s", shader_user_path);
		}
		return nullptr;
	}
	Shader* ExportedMaterial::CreateShaderFromRuntimeAsset(const char *shader_user_path) {
		return nullptr;
	}
	void ExportedMaterial::NewMaterialWithOnPassShader(lua_State*L) {
		const char*shader_user_path = lua_tostring(L,1);
		auto cachedMatIter = mCachedMaterials.find(shader_user_path);
		if (cachedMatIter != mCachedMaterials.end()) {
			cachedMatIter->second->PushUserData(L);
			lua_pushboolean(L, false);
			return;
		}
		Shader * shader = Shader::GetCached(shader_user_path);
		if (shader == nullptr){
#if _DEBUG || DEV_PUBLISH
			shader=CreateShaderFromFile(shader_user_path);
#else
			shader = CreateShaderFromRuntimeAsset(shader_user_path);
#endif
		}
		if (shader == nullptr) {
			lua_pushnil(L);
			lua_pushboolean(L, false);
			return;
		}
		ExportedMaterial*material = Alice::CreateExportedAliceType<Alice::ExportedMaterial>();
		RenderPass * pass = Alice::CreateExportedAliceType<ExportedRenderPass>();
		pass->SetShader(shader);
		material->AddPass(pass);
		material->mbSharedMaterial = true;
		material->mName = shader_user_path;
		material->PushUserData(L);
		material->retain();
		mCachedMaterials.insert(std::pair<std::string, ExportedMaterial*>(shader_user_path, material));
		lua_pushboolean(L, true);
	}
	void ExportedMaterial::NewEmptyMaterial(lua_State*L) {
		const char*material_name = lua_tostring(L, 1);
		auto cachedMatIter = mCachedMaterials.find(material_name);
		if (cachedMatIter != mCachedMaterials.end()) {
			cachedMatIter->second->PushUserData(L);
			lua_pushboolean(L, false);
			return;
		}
		ExportedMaterial*material = Alice::CreateExportedAliceType<Alice::ExportedMaterial>();
		material->mbSharedMaterial = true;
		material->mName = material_name;
		material->PushUserData(L);
		material->retain();
		lua_pushboolean(L, true);
		mCachedMaterials.insert(std::pair<std::string, ExportedMaterial*>(material_name, material));
	}
	int ExportedMaterial::API_New(lua_State*L){
		if (lua_gettop(L)==2){
			if (lua_isboolean(L,2)&&lua_toboolean(L,2)){
				NewMaterialWithOnPassShader(L);
			}
			else {
				NewEmptyMaterial(L);
			}
		} else {
			NewMaterialWithOnPassShader(L);
		}
		return 2;
	}
}
