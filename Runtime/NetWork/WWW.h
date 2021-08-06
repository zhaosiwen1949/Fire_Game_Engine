#pragma once
#include "HttpWorker.h"
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Thread/ScheduleUnit.h"
#include <iostream>
extern "C" {
#include "External/LibCurl/include/curl/curl.h"
}
#include "Runtime/String/FixedString.h"
namespace Alice{
	class WWW:public HttpWorker {
	public:
		double mProgress;
		FixedString mLocalFilePath, mUrl,mFileMD5;
		FixedString mServerName;
		AliceSInt64 mDownloadedFileSize;
		AliceSInt64 mTotalFileSize;
		int mPreferedIPVersion;
		long timeout;
		CURLcode DownloadFile();
		AliceSInt64 GetTargetFileSize();
		bool IsDownloadSucessed();
	protected:
		void Body();
		void Update();
	private:
		static int Processing(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);
		static size_t OnReceiveData(void *ptr, size_t size, size_t nmemb, void *userdata);
		static size_t OnReceivedHeaderData(void *ptr, size_t size, size_t nmemb, void *userdata);
	public:
		WWW();
		~WWW();
		DECLEAR_ALICE_CLASS(DownloadModule)
	};
}