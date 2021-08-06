#include "ExportedImageSprite.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Exported/Render/ExportedCamera.h"
#include "Runtime/Exported/Render/ExportedTexture2D.h"
#include "Runtime/Graphics/QRCodeImage.h"

namespace Alice{
	int ExportedImageSprite::API_SetTexture(lua_State*L){
		int argc = lua_gettop(L);
		if (argc==2){
			ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
			if (lua_isstring(L, 2)) {
				ptr->SetTexture("U_MainTexture", lua_tostring(L,2));
			}else if(lua_isuserdata(L,2)){
				ptr->SetTexture("U_MainTexture", TO_USERDATA(L,ExportedTexture2D,2));
			}
			else {
				Error("SetTexture failed %s",LUA_TYPE(L,2));
			}
		}else if (argc==3){
			ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
			if (lua_isstring(L, 2) && lua_isstring(L, 3)) {
				ptr->SetTexture(lua_tostring(L, 2), lua_tostring(L, 3));
			}else{
				Error("not implemented yet %s",LUA_TYPE(L,2), LUA_TYPE(L, 3));
			}
		}
		return 0;
	}
	int ExportedImageSprite::API_GetImagePath(lua_State*L) {
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		lua_pushstring(L, ptr->mImage.mText);
		return 1;
	}

	int ExportedImageSprite::API_SetCameraColorBuffer(lua_State*L) {
		int argc = lua_gettop(L);
		if (argc == 2) {
			ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
			if (lua_isuserdata(L, 2)) {
				ExportedCamera*camera = TO_USERDATA(L,ExportedCamera,2);
				ptr->SetTexture("U_MainTexture", camera->mFBO->mColorBuffer.mPtr);
			}
		}
		return 0;
	}
	int ExportedImageSprite::API_SetCameraDepthColorBuffer(lua_State*L) {
		int argc = lua_gettop(L);
		if (argc == 2) {
			ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
			if (lua_isuserdata(L, 2)) {
				ExportedCamera*camera = TO_USERDATA(L, ExportedCamera, 2);
				ptr->SetTexture("U_MainTexture", camera->mDepthFBO->mColorBuffer.mPtr);
			}
		}
		return 0;
	}

	int ExportedImageSprite::API_SetSprite(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		if (lua_isstring(L, 2)){
			const char* spriteName = lua_tostring(L, 2);
			ptr->SetSpriteByName(spriteName);
		}else{
			Error("image sprite set sprite except a string value");
			return 0;
		}
		return 0;
	}

	int ExportedImageSprite::API_SetMaterial(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		if (IsType<Material>(L, 2)){
			Material*mat = TO_USERDATA(L, Material, 2);
			ptr->SetMaterial(mat);
		}else{
			errorC("second arg must be a material instance %s",LUA_TYPE(L, 2));
			return 0;
		}
		return 0;
	}

	int ExportedImageSprite::API_GetMaterial(lua_State*L)
	{
		ExportedImageSprite*is = TO_USERDATA(L, ExportedImageSprite, 1);
		is->mMaterial->PushUserData(L);
		return 1;
	}

	int ExportedImageSprite::API_SetColor(lua_State*L){
		CheckArgCount(ExportedImageSprite::SetColor, L, 5);
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->SetColor((float)lua_tointeger(L, 2)/255.0f, (float)lua_tointeger(L, 3) / 255.0f, (float)lua_tointeger(L, 4) / 255.0f, (float)lua_tointeger(L, 5) / 255.0f);
		return 0;
	}
	int ExportedImageSprite::API_SetAlpha(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->SetAlpha(lua_tointeger(L, 2));
		return 0;
	}
	int ExportedImageSprite::API_SetSize(lua_State*L){
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3)){
			errorC("ImageSprite:SetSize arg at 2/3 must be a number");
			return 0;
		}
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->SetWidth(lua_tonumber(L, 2));
		ptr->SetHeight(lua_tonumber(L, 3));
		return 0;
	}
	int ExportedImageSprite::API_GetSize(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		lua_pushnumber(L, ptr->mWidth);
		lua_pushnumber(L, ptr->mHeight);
		return 2;
	}
	int ExportedImageSprite::API_SetRGBData(lua_State*L) {
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		const char * data = lua_tostring(L, 2);
		int width = lua_tointeger(L, 3);
		int height = width;
		if (lua_gettop(L) == 4) {
			height = lua_tointeger(L, 4);
		}
		Texture2D*texture = new Texture2D;
		texture->InitTexture2D(width, height,GL_RGB,GL_RGB);
		texture->UploadTexture((unsigned char*)data);
		ptr->SetTexture("U_MainTexture", texture);
		texture->mName = "RGBImage";
		return 0;
	}
	int ExportedImageSprite::API_SetRGBAData(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		const char * data = lua_tostring(L, 2);
		int width = lua_tointeger(L, 3);
		int height = width;
		if (lua_gettop(L)==4){
			height = lua_tointeger(L, 4);
		}
		Texture2D*texture = new Texture2D;
		texture->InitTexture2D(width, height);
		texture->UploadTexture((unsigned char*)data);
		ptr->SetTexture("U_MainTexture", texture);
		texture->mName = "RGBAImage";
		return 0;
	}
	int ExportedImageSprite::API_SetQRCodeData(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		const char * str = lua_tostring(L, 2);
		int len = strlen(str);
		int size = lua_tointeger(L, 3);
		unsigned char *image_data = new unsigned char[size*size*4];
		QRCodeImage::String2Image(str, len, image_data, size);
		Texture2D*texture = new Texture2D;
		texture->InitTexture2D(size, size);
		texture->UploadTexture(image_data);
		ptr->SetTexture("U_MainTexture",texture);
		texture->mName = "QRCode";
		delete[] image_data;
		return 0;
	}
	int ExportedImageSprite::API_SetImageData(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		const char * data = lua_tostring(L, 2);
		int data_len = lua_tointeger(L, 3);
		Texture2D*texture = Texture2D::CreateTextureFromMemory(data,data_len);
		if (texture != Texture2D::mTextureError) {
			ptr->SetTexture("U_MainTexture", texture);
			texture->mName = "ImageTexture";
		}
		return 0;
	}

	int ExportedImageSprite::API_UpdateNormal(lua_State*L){
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->UpdateVertexNormal(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}

	int ExportedImageSprite::API_UpdateTangent(lua_State*L)
	{
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->UpdateVertexTangent(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}

	int ExportedImageSprite::API_UpdateTexcoord1(lua_State*L)
	{
		ExportedImageSprite*ptr = TO_USERDATA(L, ExportedImageSprite, 1);
		ptr->UpdateVertexTexcoord1(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}
	int ExportedImageSprite::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetCameraColorBuffer)
			EXPORT_API(SetCameraDepthColorBuffer)
			EXPORT_API(SetTexture)
			EXPORT_API(SetImageData)
			EXPORT_API(SetRGBData)
			EXPORT_API(SetRGBAData)
			EXPORT_API(SetQRCodeData)
			EXPORT_API(GetImagePath)
			EXPORT_API(SetMaterial)
			EXPORT_API(GetMaterial)
			EXPORT_API(SetSprite)
			EXPORT_API(SetColor)
			EXPORT_API(SetAlpha)
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(UpdateNormal)
			EXPORT_API(UpdateTangent)
			EXPORT_API(UpdateTexcoord1)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, ImageSprite, apis);
		SET_PARENT(L, Component2D);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, ImageSprite, api_c);
		return 0;
	}
}
