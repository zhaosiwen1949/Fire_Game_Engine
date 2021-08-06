#pragma once
#include "Runtime/Animation/Avatar.h"
namespace Alice{
	class ExportedAvatar :public Avatar {
	public:
		DEFINE_LUA_API(SetMaterial);
		DEFINE_LUA_API(SetAvatarData);
		DEFINE_LUA_API(AttachAnimation);
		DEFINE_LUA_API(Play);
		DEFINE_LUA_API(SetAnimationMode);
		DEFINE_LUA_API(GetAnimationList);
		DEFINE_LUA_API(EnableDebug);
		DEFINE_LUA_API(SetAnimationTime);
		DEFINE_LUA_API(SetAnimationCallback);
		DEFINE_LUA_API(SetBoundingVolume);
		DEFINE_LUA_API(GetBoundingVolume);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedAvatar, Avatar)
	};
}
