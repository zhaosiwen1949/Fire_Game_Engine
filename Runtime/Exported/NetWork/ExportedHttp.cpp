#include "ExportedHttp.h"

namespace Alice{
	void ExportedHttp::OnSucess(void*param) {
		if (Lua_GetFunction(mCallback)) {
			Lua_PushInt(mErrorCode);
			if (mErrorCode == CURLcode::CURLE_OK) {
				Lua_PushLString(mResponseData.mText, mResponseData.mLen);
			}
			Lua_PushInt(mSendBufferSize);
			Lua_PushInt(mReceiveBufferSize);
			Lua_Invoke("On Http Sucess : %s");
		}
	}
	void ExportedHttp::OnError(void*param) {
		if (Lua_GetFunction(mCallback)) {
			Lua_PushInt(mErrorCode);
			Lua_PushInt(mSendBufferSize);
			Lua_PushInt(mReceiveBufferSize);
			Lua_Invoke("On Http Error : %s");
		}
	}
	void ExportedHttp::OnProgress(void*param) {
	}
	int ExportedHttp::API_AsyncGet(lua_State*L){
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		ptr->SetURL(lua_tostring(L,2));
		ptr->mListener = ptr;
		ptr->mMethod = kHttpCurlMethodGet;
		ptr->AsyncInvoke();
		return 0;
	}
	//暂时不要调用Post接口，Post的方式比较复杂，所以基本还是由脚本层自己实现
	int ExportedHttp::API_AsyncPost(lua_State*L){
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		ptr->SetURL(lua_tostring(L, 2));
		const char * post_data = lua_tostring(L, 3);
		ptr->mPostData.Resize(strlen(post_data)+1);
		ptr->mPostData = post_data;
		ptr->mListener = ptr;
		ptr->mMethod = kHttpCurlMethodPost;
		ptr->AsyncInvoke();
		return 0;
	}
	int ExportedHttp::API_Release(lua_State*L){
		//这里删除的是指向物体的指针，会调用析构函数
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		ptr->Destroy();
		return 0;
	}
	int ExportedHttp::API_GetCookieFromServer(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		lua_pushlstring(L, ptr->mServerCookie.mText, ptr->mServerCookie.mLen);
		return 1;
	}
	int ExportedHttp::API_SetPreferedIPVersion(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		int ip_version = lua_tointeger(L, 2);
		if (ip_version==4){
			ptr->mPreferedIPVersion = CURL_IPRESOLVE_V4;
		}
		else if (ip_version == 6) {
			ptr->mPreferedIPVersion = CURL_IPRESOLVE_V6;
		}
		return 0;
	}
	int ExportedHttp::API_GetConnectTime(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		lua_pushnumber(L, ptr->mConnectTime);
		return 1;
	}
	int ExportedHttp::API_GetNSLookUpTime(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		lua_pushnumber(L, ptr->mNSLookUpTime);
		return 1;
	}
	int ExportedHttp::API_GetPreTransferTime(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		lua_pushnumber(L, ptr->mPreTransferTime);
		return 1;
	}
	int ExportedHttp::API_GetTotalTime(lua_State*L) {
		ExportedHttp*ptr = TO_USERDATA(L, ExportedHttp, 1);
		lua_pushnumber(L, ptr->mTotalTime);
		return 1;
	}
	int ExportedHttp::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(AsyncGet)
			EXPORT_API(AsyncPost)
			EXPORT_API(Release)
			EXPORT_API(GetCookieFromServer)
			EXPORT_API(SetPreferedIPVersion)
			EXPORT_API(GetConnectTime)
			EXPORT_API(GetNSLookUpTime)
			EXPORT_API(GetPreTransferTime)
			EXPORT_API(GetTotalTime)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, HttpCurl, apis);
		SET_PARENT(L, HttpWorker);
		luaL_Reg api_c[] ={
			{"New",New_ExportedAliceType},
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Http, api_c);
		return 0;
	}
	int ExportedHttp::New_ExportedAliceType(lua_State*L){
		ExportedHttp**pp = (ExportedHttp**)lua_newuserdata(L, sizeof(ExportedHttp*));//创建一个指向由lua虚拟机管理的指针
		*pp = new (kMemDefaultId)ExportedHttp;//真正的物体指针在这里
		ExportedHttp*pObject = *pp;
		SET_LUA_TYPE(L, HttpCurl);
		pObject->GetUserData(L);//这里引用lua虚拟机管理的指针，让lua虚拟机不要调用这个物体的GC方法
		if (lua_isnumber(L,1)){
			pObject->SetReponseDataSize(lua_tointeger(L,1));
		}
		else {
			pObject->SetReponseDataSize(102400);
		}
		return 1;
	}
}
