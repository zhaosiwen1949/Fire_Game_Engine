#pragma once
//基于libcurl的http类
extern "C" {
#include "External/LibCurl/include/curl/curl.h"
}
#include "HttpWorker.h"
#include "Runtime/Thread/ScheduleUnit.h"
using namespace std;
namespace Alice {
	enum HttpCurlMethod {
		kHttpCurlMethodNone,
		kHttpCurlMethodGet,
		kHttpCurlMethodPost,
		kHttpCurlMethodCount
	};
	class HttpCurl:public HttpWorker {
	public:
		HttpCurl();
		virtual ~HttpCurl();
		FixedString mUrl, mPostData, mResponseData;
		FixedString mServerCookie;
		HttpCurlMethod mMethod;
		int mPreferedIPVersion;
		double mTotalTime,mNSLookUpTime,mConnectTime,mPreTransferTime;
		void SetURL(const char *url);
		void SetReponseDataSize(int size);
	protected:
		void Body();
		void Update();
	private:
		static void InitPostCurl(CURL*curl,HttpCurl*http);
		static void InitGetCurl(CURL*curl, HttpCurl*http);
		static size_t OnReceivedHeaderData(void *ptr, size_t size, size_t nmemb, void *userdata);
		static size_t OnReceiveBodyData(void *ptr, size_t size, size_t nmemb, void *userdata);
		static int OnDebugFunction(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr);
	public:
		DECLEAR_ALICE_CLASS(HttpCurl)
	};
}