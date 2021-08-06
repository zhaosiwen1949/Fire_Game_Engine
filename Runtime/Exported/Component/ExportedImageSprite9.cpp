#include "ExportedImageSprite9.h"
#include "Runtime/Render/AliceGL.h"

namespace Alice
{
	int ExportedImageSprite9::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetTexture)
			EXPORT_API(SetMaterial)
			EXPORT_API(GetMaterial)
			EXPORT_API(SetSprite)
			EXPORT_API(SetColor)
			EXPORT_API(SetAdditionalColor)
			EXPORT_API(SetAlpha)
			EXPORT_API(SetSize)
			EXPORT_API(GetSize)
			EXPORT_API(SetCenterRect)
			//EXPORT_API(GetCenterRect)
			EXPORT_API(UpdateNormal)
			EXPORT_API(UpdateTangent)
			EXPORT_API(UpdateTexcoord1)
			EXPORT_API(BlendFunc)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, ImageSprite9, apis);
		SET_PARENT(L, Component2D);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, ImageSprite9, api_c);
		return 0;
	}

	int ExportedImageSprite9::API_SetTexture(lua_State*L)
	{
		int argc = lua_gettop(L);
		if (argc == 2) {
			ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
			if (lua_isstring(L, 2)) {
				ptr->SetTexture("U_MainTexture", lua_tostring(L, 2));
			}
		}
		else if (argc == 3) {
			ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
			if (lua_isstring(L, 2) && lua_isstring(L, 3)) {
				ptr->SetTexture(lua_tostring(L, 2), lua_tostring(L, 3));
			}
			else {
				Error("not implemented yet %s", LUA_TYPE(L, 2), LUA_TYPE(L, 3));
			}
		}
		return 0;
	}

	int ExportedImageSprite9::API_SetCenterRect(lua_State*L){
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->mLeftMargin = lua_tonumber(L,2);
		ptr->mRightMargin = lua_tonumber(L, 3);
		ptr->mTopMargin = lua_tonumber(L, 4);
		ptr->mBottomMargin = lua_tonumber(L, 5);
		ptr->SetSplit(ptr->mTopMargin, ptr->mRightMargin, ptr->mBottomMargin, ptr->mLeftMargin);
		return 0;
	}

	int ExportedImageSprite9::API_BlendFunc(lua_State*L) {
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->BlendFunc(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}

	int ExportedImageSprite9::API_SetSprite(lua_State*L){
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		if (lua_isstring(L, 2)){
			const char* spriteName = lua_tostring(L, 2);
			ptr->SetSpriteByName(spriteName);
		}else{
			Error("image sprite set sprite except a string value");
			return 0;
		}
		return 0;
	}

	int ExportedImageSprite9::API_SetMaterial(lua_State*L){
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		if (lua_isuserdata(L, 2)){
			//set resource
			Material*mat = TO_USERDATA(L, Material, 2);
			ptr->SetMaterial(mat);
		}else if (lua_isstring(L, 2)){
			//set material internal path
			ptr->SetMaterial(lua_tostring(L, 2));
		}else{
			errorC("second arg must be material user data");
			return 0;
		}
		return 0;
	}

	int ExportedImageSprite9::API_GetMaterial(lua_State*L)
	{
		ExportedImageSprite9*is = TO_USERDATA(L, ExportedImageSprite9, 1);
		is->mMaterial->PushUserData(L);
		return 1;
	}

	int ExportedImageSprite9::API_SetColor(lua_State*L)
	{
		CheckArgCount(ExportedImageSprite9::SetColor, L, 5);
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->SetColor((float)lua_tointeger(L, 2)/255.0f, (float)lua_tointeger(L, 3) / 255.0f, (float)lua_tointeger(L, 4) / 255.0f, (float)lua_tointeger(L, 5) / 255.0f);
		return 0;
	}
	int ExportedImageSprite9::API_SetAdditionalColor(lua_State*L)
	{
		CheckArgCount(ExportedImageSprite9::SetColor, L, 5);
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->SetAdditionalColor((float)lua_tointeger(L, 2) / 255.0f, (float)lua_tointeger(L, 3) / 255.0f, (float)lua_tointeger(L, 4) / 255.0f, (float)lua_tointeger(L, 5) / 255.0f);
		return 0;
	}

	int ExportedImageSprite9::API_SetAlpha(lua_State*L)
	{
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->SetAlpha(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedImageSprite9::API_SetSize(lua_State*L)
	{
		if (!lua_isnumber(L,2)||!lua_isnumber(L,3))
		{
			errorC("ImageSprite9:SetSize arg at 2/3 must be a number");
			return 0;
		}
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->SetWidth(lua_tonumber(L, 2));
		ptr->SetHeight(lua_tonumber(L, 3));
		return 2;
	}

	int ExportedImageSprite9::API_GetSize(lua_State*L)
	{
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		lua_pushnumber(L, ptr->mWidth);
		lua_pushnumber(L, ptr->mHeight);
		return 2;
	}

	int ExportedImageSprite9::API_UpdateNormal(lua_State*L) {
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->UpdateVertexNormal(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}

	int ExportedImageSprite9::API_UpdateTangent(lua_State*L)
	{
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->UpdateVertexTangent(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}

	int ExportedImageSprite9::API_UpdateTexcoord1(lua_State*L)
	{
		ExportedImageSprite9*ptr = TO_USERDATA(L, ExportedImageSprite9, 1);
		ptr->UpdateVertexTexcoord1(lua_tointeger(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
		return 0;
	}
}