#include "ExportedAvatar.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/Exported/Scene/ExportedGameObject.h"
namespace Alice{
	int ExportedAvatar::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(Play)
			EXPORT_API(EnableDebug)
			EXPORT_API(SetAnimationTime)
			EXPORT_API(SetAnimationMode)
			EXPORT_API(GetAnimationList)
			EXPORT_API(SetAnimationCallback)
			EXPORT_API(SetMaterial)
			EXPORT_API(SetAvatarData)
			EXPORT_API(AttachAnimation)
			EXPORT_API(SetBoundingVolume)
			EXPORT_API(GetBoundingVolume)
			EXPORT_API(ClassID)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Avatar, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = { 
			EXPORT_API(ClassID)
			EXPORT_API_END 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Avatar, api_c);
		lua_pushnumber(L, kAnimationTypeOnce);
		lua_setfield(L, -2, "AnimationTypeOnce");
		lua_pushnumber(L, kAnimationTypePingpong);
		lua_setfield(L, -2, "AnimationTypePingpong");
		lua_pushnumber(L, kAnimationTypeRepeated);
		lua_setfield(L, -2, "AnimationTypeRepeated");
		return 0;
	}
	int ExportedAvatar::API_GetAnimationList(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		int nIndex = 1;
		lua_newtable(L);
		for (auto iter = ptr->mAnimations.begin(); iter != ptr->mAnimations.end(); ++iter) {
			lua_pushinteger(L, nIndex++);
			lua_pushstring(L, iter->first.c_str());
			lua_settable(L, -3);
		}
		return 1;
	}
	int ExportedAvatar::API_Play(lua_State*L){
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->Play(lua_tostring(L,2));
		return 0;
	}
	int ExportedAvatar::API_SetAnimationCallback(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		if (ptr->mAnimationCallback != LUA_REFNIL) {
			lua_unref(L, ptr->mAnimationCallback);
			ptr->mAnimationCallback = LUA_REFNIL;
		}
		if (lua_isfunction(L, -1)) {
			ptr->mAnimationCallback = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		else {
			ptr->mAnimationCallback = LUA_REFNIL;
		}
		return 0;
	}
	int ExportedAvatar::API_SetMaterial(lua_State*L){
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		int meshIndex = lua_tointeger(L, 2);
		ExportedMaterial*material = TO_USERDATA(L, ExportedMaterial, 3);
		if (meshIndex == -1) {
			ptr->mSharedMaterial = material;
		}
		else {
			ptr->SetMaterial(meshIndex, material);
		}
		return 0;
	}

	int ExportedAvatar::API_SetAvatarData(lua_State*L){
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->SetAvatarData(lua_tostring(L, 2));
		return 0;
	}

	int ExportedAvatar::API_AttachAnimation(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->AttachAnimation(lua_tostring(L, 2));
		return 0;
	}
	int ExportedAvatar::API_SetBoundingVolume(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->mLocalBoundingVolume.mCenter.Set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		ptr->mLocalBoundingVolume.mRadius = lua_tonumber(L, 5);
		return 0;
	}
	int ExportedAvatar::API_GetBoundingVolume(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		lua_pushnumber(L, ptr->mLocalBoundingVolume.mCenter.x);
		lua_pushnumber(L, ptr->mLocalBoundingVolume.mCenter.y);
		lua_pushnumber(L, ptr->mLocalBoundingVolume.mCenter.z);
		lua_pushnumber(L, ptr->mLocalBoundingVolume.mRadius);
		return 4;
	}
	int ExportedAvatar::API_EnableDebug(lua_State*L) {
		return 0;
	}
	int ExportedAvatar::API_SetAnimationTime(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->mCurrentAnimation->mAnimationTime += lua_tonumber(L, 2);
		ptr->mCurrentAnimation->mAnimationTime < 0.0f ? 0.0f : ptr->mCurrentAnimation->mAnimationTime;
		return 0;
	}
	int ExportedAvatar::API_SetAnimationMode(lua_State*L) {
		ExportedAvatar*ptr = TO_USERDATA(L, ExportedAvatar, 1);
		ptr->mAnimationType = (AnimationType)lua_tointeger(L, 2);
		return 0;
	}
}