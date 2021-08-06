#include "ExportedAudioResource.h"
namespace Alice{
	int ExportedAudioResource::API_Dump(lua_State*L){
		return 0;
	}
	int ExportedAudioResource::API_SetRawDataSize(lua_State*L){
		ExportedAudioResource*ptr = TO_USERDATA(L,ExportedAudioResource, 1);
		ptr->mRawDataBufferSize = lua_tointeger(L,2);//mp3 file size
		ptr->mRawData = new (kMemAudioId)AliceUInt8[ptr->mRawDataBufferSize];
		memset(ptr->mRawData, 0, ptr->mRawDataBufferSize);
		ptr->mCachedRawDataSize = 0;
		return 0;
	}
	int ExportedAudioResource::API_FeedData(lua_State*L){
		ExportedAudioResource*ptr = TO_USERDATA(L, ExportedAudioResource, 1);
		ptr->Feed((AliceUInt8*)lua_tostring(L, 2), lua_tointeger(L, 3));
		return 0;
	}
	int ExportedAudioResource::API_GetChannelCount(lua_State*L) {
		ExportedAudioResource*ptr = TO_USERDATA(L, ExportedAudioResource, 1);
		lua_pushinteger(L, ptr->mChannelCount);
		return 1;
	}
	int ExportedAudioResource::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(FeedData)
			EXPORT_API(SetRawDataSize)
			EXPORT_API(GetChannelCount)
			EXPORT_API(Dump)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, AudioResource, apis);
		luaL_Reg api_c[] = {
			{"New",New_ExportedAliceType},
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, AudioResource, api_c);
		return 0;
	}
}