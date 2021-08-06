#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/NetWork/WWW.h"

namespace Alice{
	class ExportedWWW :public WWW, public HttpWorkerListener {
	public:
		void OnSucess(void*param);
		void OnError(void*param);
		void OnProgress(void*param);
	public:
		DEFINE_LUA_API(AsyncDownload);
		DEFINE_LUA_API(ReDownload);
		DEFINE_LUA_API(Release);
		DEFINE_LUA_API(SetPreferedIPVersion);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedWWW, WWW)
	};
}
