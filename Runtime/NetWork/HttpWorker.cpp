#include "HttpWorker.h"
namespace Alice {
	HttpWorker::HttpWorker() {
		mErrorCode = 0;
		mSendBufferSize = 0;
		mReceiveBufferSize = 0;
		mListener = nullptr;
	}
	HttpWorker::~HttpWorker() {
	}
	void HttpWorker::SetCookie(const char *cookie) {
		int len = strlen(cookie);
		if (mCookie.mBufferSize - 1 < len) {
			mCookie.Resize(len + 1);
		}
		mCookie = cookie;
	}
	void HttpWorker::SetReferer(const char *referer) {
		int len = strlen(referer);
		if (mReferer.mBufferSize - 1 < len) {
			mReferer.Resize(len + 1);
		}
		mReferer = referer;
	}
	int HttpWorker::Export(lua_State*L) {
		luaL_Reg api_m[] = {
			EXPORT_API(SetCookie)
			EXPORT_API(SetReferer)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, HttpWorker, api_m);
		SET_PARENT(L, AliceThread);
		return 0;
	}
	int HttpWorker::API_SetCookie(lua_State*L) {
		HttpWorker*ptr = TO_USERDATA(L, HttpWorker, 1);
		ptr->SetCookie(lua_tostring(L, 2));
		return 0;
	}
	int HttpWorker::API_SetReferer(lua_State*L) {
		HttpWorker*ptr = TO_USERDATA(L, HttpWorker, 1);
		ptr->SetReferer(lua_tostring(L, 2));
		return 0;
	}
}