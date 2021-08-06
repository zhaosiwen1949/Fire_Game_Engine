#pragma once
//导出的多线程，让程序可以真多线程执行代码
#include "Runtime/Base/Object.h"
using namespace std;
namespace Alice {
	//线程只管执行，不管结果如何，结果由主线程直接查询
	enum ThreadState {
		kThreadStateCreated,//线程刚被创建
		kThreadStateBusy,//线程正在执行
		kThreadStateEnd,//线程执行完毕
		kThreadStateRelease,//线程可以被释放了
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
		//由主线程进行轮询、如果线程处于End或者Failed状态，则进行回调
		//各种异步执行的回调会通过重载，进行差异化处理
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