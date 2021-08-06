#include "ExportedWWW.h"
namespace Alice {
	void ExportedWWW::OnSucess(void*param) {
		if (Lua_GetFunction(mCallback)) {
			Lua_PushString("Sucess");
			Lua_PushUserData(mLuaData);
			Lua_PushString(mLocalFilePath.mText);
			Lua_PushString(mFileMD5.mText);
			Lua_PushInt(mSendBufferSize);
			Lua_PushInt(mReceiveBufferSize);
			Lua_Invoke("WWW::OnSucess : %s");
		}
	}
	void ExportedWWW::OnProgress(void*param) {
		if (Lua_GetFunction(mCallback)) {
			Lua_PushString("Progress");
			Lua_PushUserData(mLuaData);
			Lua_PushNumber(mProgress);
			Lua_Invoke("WWW::OnProgress : %s");
		}
	}
	void ExportedWWW::OnError(void*param) {
		if (Lua_GetFunction(mCallback)) {
			Lua_PushString("Error");
			Lua_PushUserData(mLuaData);
			Lua_PushString(mLocalFilePath.mText);
			Lua_PushString(mFileMD5.mText);
			Lua_PushInt(mErrorCode);
			Lua_PushInt(mSendBufferSize);
			Lua_PushInt(mReceiveBufferSize);
			Lua_Invoke("WWW::OnError : %s");
		}
	}
	int ExportedWWW::API_AsyncDownload(lua_State*L){
		ExportedWWW*ptr = TO_USERDATA(L, ExportedWWW, 1);
		ptr->mUrl = lua_tostring(L, 2);
		ptr->mLocalFilePath = lua_tostring(L, 3);
		ptr->mFileMD5 = lua_tostring(L, 4);
		ptr->mListener = ptr;
		ptr->AsyncInvoke();
		return 0;
	}

	int ExportedWWW::API_ReDownload(lua_State*L){
		ExportedWWW*ptr = TO_USERDATA(L, ExportedWWW, 1);
		ptr->AsyncInvoke();
		return 0;
	}
	int ExportedWWW::API_Release(lua_State*L){
		ExportedWWW*ptr = TO_USERDATA(L, ExportedWWW, 1);
		ptr->Destroy();
		return 0;
	}
	int ExportedWWW::API_SetPreferedIPVersion(lua_State*L) {
		ExportedWWW*ptr = TO_USERDATA(L, ExportedWWW, 1);
		int ip_version = lua_tointeger(L, 2);
		if (ip_version == 4) {
			ptr->mPreferedIPVersion = CURL_IPRESOLVE_V4;
		}
		else if (ip_version == 6) {
			ptr->mPreferedIPVersion = CURL_IPRESOLVE_V6;
		}
		return 0;
	}
	int ExportedWWW::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(AsyncDownload)
			EXPORT_API(ReDownload)
			EXPORT_API(Release)
			EXPORT_API(SetPreferedIPVersion)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, WWW, apis);
		SET_PARENT(L, HttpWorker);
		luaL_Reg api_c[] ={
			{"New",New_ExportedAliceType},
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, WWW, api_c);
		return 0;
	}
}
