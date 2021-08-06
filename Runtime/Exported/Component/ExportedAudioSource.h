#pragma once
#include "Runtime/Audio/AudioSource.h"

//2016 08/24 2.8.7 pass 

namespace Alice
{
	class ExportedAudioSource :public AudioSource
	{
	public:
		DEFINE_LUA_API(Play);
		DEFINE_LUA_API(Pause);
		DEFINE_LUA_API(Stop);
		DEFINE_LUA_API(PlayFrom);
		DEFINE_LUA_API(SetPlayTime);
		DEFINE_LUA_API(SetOnStopCallback);
		DEFINE_LUA_API(SetLoop);
		DEFINE_LUA_API(SetVolume);
		DEFINE_LUA_API(SetAudioClip);
		DEFINE_LUA_API(GetAudioTime);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedAudioSource, AudioSource)
	};
}
