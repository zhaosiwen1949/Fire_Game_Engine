#include "AliceThread.h"
#include <thread>
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ResourceManager.h"
namespace Alice{
	std::set<AliceThread*> AliceThread::mThreads;
	AliceThread::AliceThread(){
		mName = "AliceThreadObject";
		mState = kThreadStateCreated;
		mCallback = LUA_REFNIL;
		mThreads.insert(this);
	}
	AliceThread::~AliceThread(){
		if (mCallback!=LUA_REFNIL){
			lua_unref(Alice::LuaEngine::s_GlobalStatePtr, mCallback);
			mCallback = LUA_REFNIL;
		}
	}
	void AliceThread::AsyncInvoke() {
		mState = kThreadStateBusy;
		std::thread t(&AliceThread::Body, this);
		t.detach();
	}
	void AliceThread::Destroy() {
		mState = kThreadStateRelease;
	}
	void AliceThread::UpdateAliceThreads() {
		auto iter = mThreads.begin();
		auto end_iter = mThreads.end();
		for (; iter != end_iter;) {
			if ((*iter)->mState != kThreadStateRelease) {
				(*iter)->Update();
				++iter;
			}
			else {
				auto *t = *iter;
				iter = mThreads.erase(iter);
				delete t;
			}
		}
	}
	int AliceThread::Export(lua_State*L) {
		luaL_Reg api_m[] = {
			EXPORT_API(SetCallback)
			EXPORT_API(GetState)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, AliceThread, api_m);
		luaL_Reg api_c[] = {
			{NULL,NULL}
		};
		lua_pushnumber(L, kThreadStateCreated);
		lua_setfield(L, -2, "StateCreated");
		lua_pushnumber(L, kThreadStateBusy);
		lua_setfield(L, -2, "StateBusy");
		lua_pushnumber(L, kThreadStateEnd);
		lua_setfield(L, -2, "StateEnd");
		return 0;
	}

	int AliceThread::API_SetCallback(lua_State*L) {
		AliceThread*ptr = TO_USERDATA(L, AliceThread, 1);
		if (ptr->mCallback!=LUA_REFNIL){
			lua_unref(L, ptr->mCallback);
		}
		if (lua_isfunction(L, -1)) {
			ptr->mCallback = luaL_ref(L, LUA_REGISTRYINDEX);
		}else {
			ptr->mCallback = LUA_REFNIL;
		}
		return 0;
	}
	int AliceThread::API_GetState(lua_State*L) {
		AliceThread*ptr = TO_USERDATA(L, AliceThread, 1);
		lua_pushinteger(L, ptr->mState);
		return 1;
	}
	AsyncCommand::AsyncCommand() {
		mAction = kAsyncCommandActionNone;
	}
	void AsyncCommand::ExecuteCommand() {
#if ALICE_PLATFORM_WIN
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (CreateProcessA(NULL, mCommand.mText, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
#else
#ifndef ALICE_IPHONE
		system(mCommand.mText);
#endif
#endif
	}
	void AsyncCommand::CopyFiles() {
		int nCount = mSrcs.size();
		char srcpath[256];
		char dstpath[256];
		Data srcData;
		for (int i=0;i<nCount;++i){
			memset(srcpath, 0, 256);
			memset(dstpath, 0, 256);
			sprintf(srcpath, "%s/%s", mSrcFolder.mText, mSrcs[i].c_str());
			sprintf(dstpath, "%s/%s", mDstFolder.mText, mDsts[i].c_str());
			if (FileSystem::LoadDataFromPath(srcpath,srcData)){
				FileSystem::SaveData(dstpath, srcData);
			}
		}
		mSrcs.clear();
		mDsts.clear();
	}
	void AsyncCommand::DeleteFiles() {
		int nCount = mSrcs.size();
		char srcpath[256];
		for (int i = 0; i < nCount; ++i) {
			memset(srcpath, 0, 256);
			sprintf(srcpath, "%s/%s", mSrcFolder.mText, mSrcs[i].c_str());
			if (FileSystem::Exists(srcpath)) {
				ResourceManager::DeleteFileWithPath(srcpath);
			}
		}
		mSrcs.clear();
	}
	void AsyncCommand::Body(){
		switch (mAction)
		{
		case Alice::kAsyncCommandActionNone:
			break;
		case Alice::kAsyncCommandActionSystemCommand:
			ExecuteCommand();
			break;
		case Alice::kAsyncCommandActionCopyFiles:
			CopyFiles();
			break;
		case Alice::kAsyncCommandActionDeleteFiles:
			DeleteFiles();
			break;
		case Alice::kAsyncCommandActionCount:
			break;
		default:
			break;
		}
		mState = kThreadStateEnd;
	}
	void AsyncCommand::Update() {
		if (mState!=kThreadStateEnd){
			return;
		}
		switch (mState){
		case Alice::kThreadStateCreated:
			break;
		case Alice::kThreadStateBusy:
			break;
		case Alice::kThreadStateEnd:
			if (Lua_GetFunction(mCallback)) {
				Lua_PushInt(mAction);
				Lua_Invoke("AsyncCommandThread : %s");
			}
			break;
		case Alice::kThreadStateCount:
			break;
		default:
			break;
		}
		if (mState == kThreadStateRelease) {
			return;
		}
		mState = kThreadStateCreated;
	}
}
