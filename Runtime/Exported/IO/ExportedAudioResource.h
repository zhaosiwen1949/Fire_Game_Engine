#pragma once
#include "Runtime/IO/AudioResource.h"
namespace Alice{
	class ExportedAudioResource :public AudioResource{
	public:
		DEFINE_LUA_API(FeedData);
		DEFINE_LUA_API(SetRawDataSize);
		DEFINE_LUA_API(GetChannelCount);
		DEFINE_LUA_API(Dump);
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedAudioResource, AudioResource)
	};
}
