#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Thread/AliceThread.h"
namespace Alice {
	enum HttpWorkerState {
		kHttpWorkerStateOK,//û��ʲô�쳣��״̬����ʼ��״̬����OK״̬
		kHttpWorkerStateError,//�����߳�������������󣬻Ὣ״̬�޸ĳ�Ϊ���״̬
		kHttpWorkerStateErrorCallbackInvoked,//�����߳��У�����������߳��е�״̬Ϊ����״̬����ô��������߳���Ļص���Ȼ���޸�״̬
		kHttpWorkerStateSucess,//�����߳��У������ȷ��ִ���ˣ���ô����״̬�޸ĳ�Ϊִ�гɹ�
		kHttpWorkerStateSucessCallbackInvoked,//�����߳��У���������˴�״̬�����������߳���Ļص���Ȼ���޸�״̬
		kHttpWorkerStateCount
	};
	class HttpWorkerListener {
	public:
		virtual void OnSucess(void*param) = 0;
		virtual void OnError(void*param) = 0;
		virtual void OnProgress(void*param) = 0;
	};
	//���Ǹ�API�ӿڣ����ڱ����ʵ��
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