#include "ExportedLabel.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Exported/Scene/ExportedGameObject.h"
#include "Runtime/Exported/Graphic/ExportedDynamicFont.h"

namespace Alice
{
	int ExportedLabel::API_SetText(lua_State*L){
		if (lua_gettop(L) != 2){
			return 0;
		}
		if (!lua_isstring(L, 2)){
			luaL_error(L, "Alice.Label:SetText arg is not string %s", lua_typename(L, lua_type(L, 2)));
			return 0;
		}
		Label*ptr = TO_USERDATA(L, Label, 1);
		const char *szSrc = lua_tostring(L, 2);
		ptr->SetText(szSrc);
		return 0;
	}

	int ExportedLabel::API_GetText(lua_State*L){
		if (lua_gettop(L) != 1){
			lua_pushnil(L);
			return 1;
		}
		Label*ptr = TO_USERDATA(L, Label, 1);
		char szBuffer[2048] = { 0 };
		int len = 0;
		ptr->GetUTF8Text(szBuffer, len);
		lua_pushstring(L, szBuffer);
		return 1;
	}
	int ExportedLabel::API_GetCharAt(lua_State*L) {
		Label*ptr = TO_USERDATA(L, Label, 1);
		char szBuffer[4] = { 0 };
		int len = 0;
		ptr->GetUTF8CharAt(lua_tointeger(L, 2)-1,szBuffer,len);
		lua_pushlstring(L, szBuffer,len);
		return 1;
	}

	int ExportedLabel::API_GetAligning(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mAligning);
		return 1;
	}

	int ExportedLabel::API_SetAligning(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetAligning((LabelAligning)lua_tointeger(L,2));
		return 0;
	}

	int ExportedLabel::API_SetCharSize(lua_State*L) {
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetCharSize(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedLabel::API_GetStyle(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mTextStyle);
		return 1;
	}

	int ExportedLabel::API_SetStyle(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetStyle((CharStyle)lua_tointeger(L, 2));
		return 0;
	}

	int ExportedLabel::API_GetColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mLabelColor.r);
		lua_pushinteger(L, ptr->mLabelColor.g);
		lua_pushinteger(L, ptr->mLabelColor.b);
		lua_pushinteger(L, ptr->mLabelColor.a);
		return 4;
	}

	int ExportedLabel::API_SetColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetColor(lua_tointeger(L,2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
		return 0;
	}

	int ExportedLabel::API_GetFont(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushstring(L, ptr->mFontName.mText);
		return 1;
	}

	int ExportedLabel::API_SetFont(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetFont(lua_tostring(L,2));
		return 0;
	}

	int ExportedLabel::API_GetOutlineColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mOutlineColor.r);
		lua_pushinteger(L, ptr->mOutlineColor.g);
		lua_pushinteger(L, ptr->mOutlineColor.b);
		lua_pushinteger(L, ptr->mOutlineColor.a);
		return 4;
	}

	int ExportedLabel::API_SetOutlineColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetOutlineColor(lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
		return 0;
	}

	int ExportedLabel::API_GetOutlineSize(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mOutlineSize);
		return 1;
	}

	int ExportedLabel::API_SetOutlineSize(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetOutlineSize(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedLabel::API_GetShadowColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mShadowColor.r);
		lua_pushinteger(L, ptr->mShadowColor.g);
		lua_pushinteger(L, ptr->mShadowColor.b);
		lua_pushinteger(L, ptr->mShadowColor.a);
		return 4;
	}

	int ExportedLabel::API_SetShadowColor(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetShadowColor(lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
		return 0;
	}

	int ExportedLabel::API_GetBold(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mShadowDistanceX);
		lua_pushinteger(L, ptr->mShadowDistanceY);
		return 2;
	}

	int ExportedLabel::API_SetBold(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetShadow(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}

	int ExportedLabel::API_GetShadow(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mShadowDistanceX);
		lua_pushinteger(L, ptr->mShadowDistanceY);
		return 2;
	}

	int ExportedLabel::API_SetShadow(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->SetShadow(lua_tointeger(L, 2), lua_tointeger(L, 3));
		return 0;
	}

	int ExportedLabel::API_GetRect(lua_State*L)
	{
		//left,bottom,width,height
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, ptr->mBox.mPoints[0].x);
		lua_pushnumber(L, ptr->mBox.mPoints[0].y);
		lua_pushnumber(L, ptr->mBox.mPoints[1].x - ptr->mBox.mPoints[0].x);
		lua_pushnumber(L, ptr->mBox.mPoints[3].y - ptr->mBox.mPoints[0].y);
		return 4;
	}
	int ExportedLabel::API_GetLocalRect(lua_State*L)
	{
		//left,bottom,width,height
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, ptr->mLeftBottom.x);
		lua_pushnumber(L, ptr->mLeftBottom.y);
		lua_pushnumber(L, ptr->mRightTop.x - ptr->mLeftBottom.x);
		lua_pushnumber(L, ptr->mRightTop.y - ptr->mLeftBottom.y);
		return 4;
	}

	int ExportedLabel::API_GetPixelLen(lua_State*L){
		//left,bottom,width,height
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, ptr->mLabelLenInPixel);
		return 1;
	}

	int ExportedLabel::API_GetHeight(lua_State*L) {
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, ptr->GetHeight());
		return 1;
	}

	int ExportedLabel::API_GetCharCount(lua_State*L) {
		//left,bottom,width,height
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushinteger(L, ptr->mCharCount);
		return 1;
	}

	int ExportedLabel::API_InsertTextAt(lua_State*L) {
		if (!lua_isstring(L, 3)) {
			errorC("label append text,arg must be a string,current arg is %s", lua_typename(L, lua_type(L, 2)));
			return 0;
		}
		Label*ptr = TO_USERDATA(L, Label, 1);
		const char *szSrc = lua_tostring(L, 3);
		if (lua_gettop(L) == 6) {
			if (strlen(szSrc) > 0) {
				UTF32Char charCode[1024] = { 0 };
				int nCount = StringUtils::UTF8ToUTF32((const unsigned char*)szSrc, strlen(szSrc), charCode);
				if (nCount > 0) {
					ptr->InsertCharAt(lua_tointeger(L, 2), charCode[0], 
						lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6),true);
				}
			}
			
		}
		else {
			if (strlen(szSrc) > 0) {
				UTF32Char charCode[1024] = { 0 };
				int nCount = StringUtils::UTF8ToUTF32((const unsigned char*)szSrc, strlen(szSrc), charCode);
				if (nCount > 0) {
					ptr->InsertCharAt(lua_tointeger(L, 2), charCode[0]);
				}
			}
		}
		return 0;
	}

	int ExportedLabel::API_AppendText(lua_State*L){
		if (!lua_isstring(L, 2)){
			errorC("label append text,arg must be a string,current arg is %s", lua_typename(L, lua_type(L, 2)));
			return 0;
		}
		Label*ptr = TO_USERDATA(L, Label, 1);
		const char *szSrc = lua_tostring(L, 2);
		if (lua_gettop(L)==6){
			ptr->AppendText(szSrc,lua_tointeger(L,3), lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6));
		}else {
			ptr->AppendText(szSrc);
		}
		return 0;
	}
	int ExportedLabel::API_TrimEnd(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->TrimEnd();
		ptr->UpdateTextPositon();
		return 0;
	}
	int ExportedLabel::API_DeleteAt(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->DeleteCharAt(lua_tointeger(L,2)-1);
		return 0;
	}

	int ExportedLabel::API_GetCharWidthInPixel(lua_State*L) {
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, ptr->GetCharWidthInPixel(lua_tointeger(L, 2)));
		return 1;
	}
	int ExportedLabel::API_GetLabelHeightInPixel(lua_State*L) {
		Label*ptr = TO_USERDATA(L, Label, 1);
		lua_pushnumber(L, FontFace::PointSize2PixelSize(ptr->mCharSizeInPoint));
		return 1;
	}
	int ExportedLabel::API_SetMaterial(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		Material*mat = TO_USERDATA(L, Material, 2);
		ptr->mMaterial = mat;
		return 0;
	}
	int ExportedLabel::API_GetMaterial(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->mMaterial->PushUserData(L);
		return 1;
	}

	int ExportedLabel::API_UseStencilMask(lua_State*L)
	{
		Label*ptr = TO_USERDATA(L, Label, 1);
		ptr->mbUseStencilMask = lua_toboolean(L, 2);
		return 0;
	}

	int ExportedLabel::API_UseFont(lua_State*L){
		return 0;
	}
	int ExportedLabel::API_SetDynamicFont(lua_State*L) {
		if (lua_isuserdata(L,2)) {
			Label*ptr = TO_USERDATA(L, Label, 1);
			ExportedDynamicFont*font = TO_USERDATA(L, ExportedDynamicFont, 2);
			ptr->mFont = font;
		}
		return 0;
	}
	int ExportedLabel::API_PointSizeToPixelSize(lua_State*L) {
		lua_pushnumber(L, FontFace::PointSize2PixelSize(lua_tointeger(L,1)));
		return 1;
	}

	int ExportedLabel::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetText)
			EXPORT_API(AppendText)
			EXPORT_API(InsertTextAt)
			EXPORT_API(TrimEnd)
			EXPORT_API(DeleteAt)
			EXPORT_API(GetCharWidthInPixel)
			EXPORT_API(GetLabelHeightInPixel)
			EXPORT_API(GetText)
			EXPORT_API(GetCharAt)
			EXPORT_API(GetLocalRect)
			EXPORT_API(GetRect)
			EXPORT_API(GetPixelLen)
			EXPORT_API(GetHeight)
			EXPORT_API(GetCharCount)
			EXPORT_API(GetStyle)
			EXPORT_API(SetStyle)
			EXPORT_API(GetAligning)
			EXPORT_API(SetAligning)
			EXPORT_API(SetCharSize)
			EXPORT_API(GetFont)
			EXPORT_API(SetFont)
			EXPORT_API(SetDynamicFont)
			EXPORT_API(GetColor)
			EXPORT_API(SetColor)
			EXPORT_API(GetShadow)
			EXPORT_API(SetShadow)
			EXPORT_API(GetShadowColor)
			EXPORT_API(SetShadowColor)
			EXPORT_API(GetOutlineSize)
			EXPORT_API(SetOutlineSize)
			EXPORT_API(GetOutlineColor)
			EXPORT_API(SetOutlineColor)
			EXPORT_API(SetMaterial)
			EXPORT_API(GetMaterial)
			EXPORT_API(UseStencilMask)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Label, apis);
		SET_PARENT(L, Component2D);
		luaL_Reg api_c[] = { 
			EXPORT_API(ClassID) 
			EXPORT_API(PointSizeToPixelSize)
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Label, api_c);
		return 0;
	}
}
