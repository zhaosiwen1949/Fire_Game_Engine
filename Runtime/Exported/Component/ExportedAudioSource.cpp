#include "ExportedAudioSource.h"
#include "Runtime/Exported/IO/ExportedAliceResource.h"

namespace Alice{
	int ExportedAudioSource::API_Play(lua_State*L){
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		if (ptr->mState == kAudioSourceStopped) {
			ptr->PrepareFrom(0);
		}
		ptr->Play();
		return 0;
	}

	int ExportedAudioSource::API_SetPlayTime(lua_State*L) {
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->PrepareFrom(lua_tointeger(L, 2));
		return 0;
	}

	int ExportedAudioSource::API_PlayFrom(lua_State*L) {
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->PrepareFrom(lua_tointeger(L, 2));
		ptr->Play();
		return 0;
	}

	int ExportedAudioSource::API_GetAudioTime(lua_State*L) {
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		lua_pushnumber(L, ptr->mAudioTime);
		return 1;
	}
	int ExportedAudioSource::API_SetVolume(lua_State*L) {
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->SetVolume(lua_tonumber(L, 2));
		return 0;
	}

	int ExportedAudioSource::API_Pause(lua_State*L){
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->Pause();
		return 0;
	}

	int ExportedAudioSource::API_Stop(lua_State*L){
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->Stop();
		return 0;
	}

	int ExportedAudioSource::API_SetOnStopCallback(lua_State*L){
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->mOnStopCallback = luaL_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}

	int ExportedAudioSource::API_SetLoop(lua_State*L)
	{
		if (!lua_isboolean(L, 2)) {
			return 0;
		}
		ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
		ptr->SetLoop(0!=lua_toboolean(L, 2));
		return 0;
	}

	int ExportedAudioSource::API_SetAudioClip(lua_State*L)
	{
		if (lua_isstring(L, 2)){
			ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
			const char *audioRes = lua_tostring(L,2);
			//audio user name
			ptr->SetAudioResource(audioRes);
			if (ptr->mAudioResource!=nullptr){
				lua_pushnumber(L, ptr->mAudioResource->GetAudioDuration());
			}else{
				lua_pushnumber(L, 0.0f);
			}
		}else if(IsType<AudioResource>(L, 2)){
			ExportedAudioSource*ptr = TO_USERDATA(L, ExportedAudioSource, 1);
			AudioResource*as = TO_USERDATA(L, AudioResource, 2);
			ptr->InitWithAudioResource(as);
			if (ptr->mAudioResource != nullptr) {
				lua_pushnumber(L, ptr->mAudioResource->GetAudioDuration());
			}
			else {
				lua_pushnumber(L, 0.0f);
			}
		}else{
			errorC("AudioSource:SetAudioClip arg at 2 must be a string or audio resource");
			lua_pushnumber(L,0.0f);
		}
		return 1;
	}

	int ExportedAudioSource::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(Play)
			EXPORT_API(PlayFrom)
			EXPORT_API(Pause)
			EXPORT_API(Stop)
			EXPORT_API(SetOnStopCallback)
			EXPORT_API(SetLoop)
			EXPORT_API(SetVolume)
			EXPORT_API(SetPlayTime)
			EXPORT_API(SetAudioClip)
			EXPORT_API(GetAudioTime)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, AudioSource, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, AudioSource, api_c);
		return 0;
	}
}