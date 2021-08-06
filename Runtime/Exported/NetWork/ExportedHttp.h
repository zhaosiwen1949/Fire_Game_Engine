#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Base/Object.h"
#include "Runtime/NetWork/HttpCurl.h"
namespace Alice{
	class ExportedHttp :public HttpCurl,public HttpWorkerListener {
	public:
		void OnSucess(void*param);
		void OnError(void*param);
		void OnProgress(void*param);
	public:
		DEFINE_LUA_API(AsyncGet);
		DEFINE_LUA_API(AsyncPost);
		DEFINE_LUA_API(Release);
		DEFINE_LUA_API(GetCookieFromServer);
		DEFINE_LUA_API(SetPreferedIPVersion);
		DEFINE_LUA_API(GetConnectTime);
		DEFINE_LUA_API(GetNSLookUpTime);
		DEFINE_LUA_API(GetPreTransferTime);
		DEFINE_LUA_API(GetTotalTime);
		static int Export(lua_State*L);
	public:
		static int New_ExportedAliceType(lua_State*L);
	};
}
