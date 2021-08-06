#pragma once
//�����Ķ��̣߳��ó����������߳�ִ�д���
#include "Runtime/Base/Object.h"
using namespace std;
namespace Alice {
	//�߳�ֻ��ִ�У����ܽ����Σ���������߳�ֱ�Ӳ�ѯ
	enum ThreadState {
		kThreadStateCreated,//�̸߳ձ�����
		kThreadStateBusy,//�߳�����ִ��
		kThreadStateEnd,//�߳�ִ�����
		kThreadStateRelease,//�߳̿��Ա��ͷ���
		kThreadStateCount
	};
	class AliceThread:public Object {
	public:
		AliceThread();
		virtual ~AliceThread();
		ThreadState mState;
		int mCallback;
		void AsyncInvoke();
		void Destroy();
		//�����߳̽�����ѯ������̴߳���End����Failed״̬������лص�
		//�����첽ִ�еĻص���ͨ�����أ����в��컯����
		static void UpdateAliceThreads();
		static std::set<AliceThread*> mThreads;
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetCallback);
		DEFINE_LUA_API(GetState);
	private:
		AliceThread(const AliceThread&r) {}
		AliceThread&operator=(const AliceThread&r) { return *this; };
	protected:
		virtual void Body() = 0;
		virtual void Update() = 0;
	}; 
	enum AsyncCommandAction{
		kAsyncCommandActionNone,
		kAsyncCommandActionSystemCommand,
		kAsyncCommandActionCopyFiles,
		kAsyncCommandActionDeleteFiles,
		kAsyncCommandActionCount
	};
	class AsyncCommand :public AliceThread {
	public:
		AsyncCommandAction mAction;
		FixedString mCommand;
		FixedString mSrcFolder;
		FixedString mDstFolder;
		vector<string> mSrcs, mDsts;
	private:
		AsyncCommand(const AsyncCommand&r) {}
		AsyncCommand&operator=(const AsyncCommand&r) { return *this; };
	public:
		AsyncCommand();
		void ExecuteCommand();
		void CopyFiles();
		void DeleteFiles();
	protected:
		void Body();
		void Update();
	public:
		DECLEAR_ALICE_CLASS(AsyncCommand)
	};
}