#include "ExportedLighting.h"

namespace Alice{
	int ExportedLight::API_SetPosition(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mPosition.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetAmbient(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mAmbientColor.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetDiffuse(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mDiffuseColor.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetSpecular(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSpecularColor.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetSetting(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetSetting1(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting1.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		return 0;
	}


	int ExportedLight::API_SetConstant(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting.y = lua_tonumber(L, 2);
		return 0;
	}

	int ExportedLight::API_SetLinear(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting.z=lua_tonumber(L,2);
		return 0;
	}

	int ExportedLight::API_SetQuadric(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting.w = lua_tonumber(L, 2);
		return 0;
	}

	int ExportedLight::API_SetCutoff(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		float cutoff = lua_tonumber(L, 2);
		if (cutoff != 1.0f&&cutoff != 0.0f) {
			light->mPosition.w = lua_tonumber(L, 2);
		}
		return 0;
	}

	int ExportedLight::API_SetExponent(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting.x = lua_tonumber(L, 2);
		return 0;
	}

	int ExportedLight::API_SetDirection(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting1.x = lua_tonumber(L, 2);
		light->mSetting1.y = lua_tonumber(L, 3);
		light->mSetting1.z = lua_tonumber(L, 4);
		return 0;
	}

	int ExportedLight::API_SetShiness(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mSetting1.w = lua_tonumber(L, 2);
		return 0;
	}

	int ExportedLight::API_SetCulling(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		light->mCullingMask=lua_tonumber(L, 2);
		return 0;
	}

	int ExportedLight::API_SetProjection(lua_State*L)
	{
		ExportedLight*ptr = TO_USERDATA(L, ExportedLight, 1);
		ptr->mProjection.SetPerspective(
			lua_tonumber(L, 2),
			lua_tonumber(L,3),
			lua_tonumber(L, 4), 
			lua_tonumber(L, 5));
		return 0;
	}
	int ExportedLight::API_SetOrtho(lua_State*L) {
		ExportedLight*ptr = TO_USERDATA(L, ExportedLight, 1);
		ptr->mProjection.SetOrtho(
			lua_tonumber(L, 2), lua_tonumber(L, 3),
			lua_tonumber(L, 4), lua_tonumber(L, 5),
			lua_tonumber(L, 6), lua_tonumber(L, 7));
		return 0;
	}
	int ExportedLight::API_LookAt(lua_State*L)
	{
		ExportedLight*ptr = TO_USERDATA(L, ExportedLight, 1);
        Vector3f eye(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
        Vector3f viewCenter(lua_tonumber(L, 5), lua_tonumber(L, 6), lua_tonumber(L, 7));
        Vector3f up(lua_tonumber(L, 8), lua_tonumber(L, 9), lua_tonumber(L, 10));
		ptr->mView.LookAt(eye,viewCenter,up);
		return 0;
	}
	int ExportedLight::API_SetType(lua_State*L)
	{
		ExportedLight*ptr = TO_USERDATA(L, ExportedLight, 1);
		ptr->mType = (LightSourceType)lua_tointeger(L, 2);
		return 0;
	}
	int ExportedLight::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(SetPosition)
			EXPORT_API(SetAmbient)
			EXPORT_API(SetDiffuse)
			EXPORT_API(SetSpecular)
			EXPORT_API(SetConstant)//for spot light and point light
			EXPORT_API(SetLinear)//for spot light and point light
			EXPORT_API(SetQuadric)//for spot light and point light
			EXPORT_API(SetCutoff)//for spot light
			EXPORT_API(SetExponent)//for spot light
			EXPORT_API(SetDirection)//for spot light
			EXPORT_API(SetShiness)//
			EXPORT_API(SetSetting)
			EXPORT_API(SetSetting1)
			EXPORT_API(SetCulling)
			EXPORT_API(SetProjection)
			EXPORT_API(SetOrtho)
			EXPORT_API(LookAt)
			EXPORT_API(SetType)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, Light, apis);
		luaL_Reg api_c[] = { 
			EXPORT_API(ClassID) 
			EXPORT_API(New)
			EXPORT_API(Delete)
			EXPORT_API(Clear)
			EXPORT_API(SetMainLight)
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Light, api_c);
		lua_pushnumber(L, 1);
		lua_setfield(L, -2, "Direction");
		lua_pushnumber(L, 2);
		lua_setfield(L, -2, "Point");
		lua_pushnumber(L, 3);
		lua_setfield(L, -2, "Spot");
		return 0;
	}
	int ExportedLight::API_New(lua_State*L){
		ExportedLight*light = Alice::CreateExportedAliceType<Alice::ExportedLight>();
		light->PushUserData(L);
		mLights.insert(light);
		return 1;
	}
	int ExportedLight::API_Delete(lua_State*L) {
		ExportedLight*light = TO_USERDATA(L,ExportedLight, 1);
		auto iter = mLights.find(light);
		if (iter!=mLights.end()){
			mLights.erase(iter);
			delete light;
		}
		return 0;
	}
	int ExportedLight::API_Clear(lua_State*L) {
		auto iter = mLights.begin();
		while (iter != mLights.end()) {
			delete *iter;
			iter++;
		}
		mLights.clear();
		return 0;
	}
	int ExportedLight::API_SetMainLight(lua_State*L) {
		if (lua_gettop(L)==0){
			Light::mMainLight = nullptr;
			return 0;
		}
		ExportedLight*light = TO_USERDATA(L, ExportedLight, 1);
		Light::mMainLight = light;
		return 0;
	}
}
