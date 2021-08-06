#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Thread/AliceThread.h"
namespace Alice {
	enum HttpWorkerState {
		kHttpWorkerStateOK,//没有什么异常的状态，初始化状态就是OK状态
		kHttpWorkerStateError,//在子线程中如果发生错误，会将状态修改成为这个状态
		kHttpWorkerStateErrorCallbackInvoked,//在主线程中，如果发现子线程中的状态为错误状态，那么会调用主线程里的回调，然后修改状态
		kHttpWorkerStateSucess,//在子线程中，如果正确的执行了，那么则会把状态修改成为执行成功
		kHttpWorkerStateSucessCallbackInvoked,//在主线程中，如果发现了此状态，则会调用主线程里的回调，然后修改状态
		kHttpWorkerStateCount
	};
	class HttpWorkerListener {
	public:
		virtual void OnSucess(void*param) = 0;
		virtual void OnError(void*param) = 0;
		virtual void OnProgress(void*param) = 0;
	};
	//这是个API接口，对于本类的实例
	class HttpWorker : public AliceThread {
	public:
		HttpWorker();
		virtual ~HttpWorker();
		int mErrorCode;
		int mSendBufferSize, mReceiveBufferSize;
		HttpWorkerListener*mListener;
		FixedString mCookie, mReferer;
		void SetCookie(const char *cookie);
		void SetReferer(const char *referer);
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetCookie);
		DEFINE_LUA_API(SetReferer);
	};
}