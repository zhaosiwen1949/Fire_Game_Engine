#include "HttpCurl.h"
#include "Runtime/Thread/MainThreadScheduler.h"
#include <thread>
#include "Runtime/Debugger/Log.h"
#include "Runtime/String/StringUtils.h"
namespace Alice{
	HttpCurl::HttpCurl(){
		mMethod = kHttpCurlMethodNone;
		mName = "HttpCurlObject";
		mServerCookie.Resize(4096);
		mPreferedIPVersion = CURL_IPRESOLVE_WHATEVER;
		mTotalTime = 0.0;
		mNSLookUpTime = 0.0;
		mConnectTime = 0.0;
		mPreTransferTime = 0.0;
	}
	HttpCurl::~HttpCurl(){
	}
	void HttpCurl::SetURL(const char *url) {
		mUrl.Resize(strlen(url) + 1);
		mUrl = url;
	}
	void HttpCurl::SetReponseDataSize(int size) {
		mResponseData.Resize(size);
	}
	void HttpCurl::Body(){
		mSendBufferSize = 0;
		mReceiveBufferSize = 0;
		CURLcode result;
		CURL* curl = curl_easy_init();
		if (nullptr == curl) {
			mErrorCode = CURLE_FAILED_INIT;
			mState = kThreadStateEnd;
			Error("init curl failed %d",mErrorCode);
			return;
		}
		if (mMethod == kHttpCurlMethodGet) {
			InitGetCurl(curl, this);
		}
		else if (mMethod == kHttpCurlMethodPost) {
			InitPostCurl(curl, this);
		}
		result = curl_easy_perform(curl);
		mErrorCode = result;
		long send_data_size;
		curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &send_data_size); 
#if ALICE_ANDROID
		double receive_data_size_x = 0.0f;
		curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &receive_data_size_x);
		int receive_data_size = int(receive_data_size_x);
#else
		curl_off_t receive_data_size = 0;
		curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &receive_data_size);
#endif
		mSendBufferSize = send_data_size;
		mReceiveBufferSize = receive_data_size;
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &mTotalTime);
		curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &mNSLookUpTime);
		curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &mConnectTime);
		curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &mPreTransferTime);
		curl_easy_cleanup(curl);
		mState = kThreadStateEnd;
	}
	void HttpCurl::Update() {
		if (mState!=kThreadStateEnd){
			return;
		}
		switch (mMethod)
		{
		case Alice::kHttpCurlMethodNone:
			break;
		case Alice::kHttpCurlMethodGet:
		case Alice::kHttpCurlMethodPost:
			mMethod = kHttpCurlMethodNone;
			if (mListener != nullptr) {
				if (mErrorCode == CURLcode::CURLE_OK) {
					mListener->OnSucess(this);
				}
				else {
					mListener->OnError(this);
				}
				if (mState==kThreadStateRelease){
					return;
				}
			}
			break;
		case Alice::kHttpCurlMethodCount:
			mMethod = kHttpCurlMethodNone;
			break;
		default:
			mMethod = kHttpCurlMethodNone;
			break;
		}
		mState = kThreadStateCreated;
	}
	void HttpCurl::InitGetCurl(CURL*curl,HttpCurl*http){
		curl_easy_setopt(curl, CURLOPT_URL, http->mUrl.mText);
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, http->mPreferedIPVersion);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpCurl::OnReceiveBodyData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, http);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HttpCurl::OnReceivedHeaderData);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, http);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebugFunction);
		if (http->mCookie.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_COOKIE, http->mCookie.mText);
		}
		if (http->mReferer.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_REFERER, http->mReferer.mText);
		}
	}
	void HttpCurl::InitPostCurl(CURL*curl, HttpCurl*http) {
		curl_easy_setopt(curl, CURLOPT_URL, http->mUrl.mText);
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, http->mPreferedIPVersion);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, http->mPostData.mText);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpCurl::OnReceiveBodyData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, http);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HttpCurl::OnReceivedHeaderData);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, http);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
		if (http->mCookie.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_COOKIE, http->mCookie.mText);
		}
		if (http->mReferer.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_REFERER, http->mReferer.mText);
		}
	}
	//header的时候，是一条一条来的，所以只需要考虑解码一行属性就可以了
	size_t HttpCurl::OnReceivedHeaderData(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
		HttpCurl*http = (HttpCurl*)lpVoid;
		int split_pos = StringUtils::GetNextPosOf((char*)buffer, size * nmemb,':');
		if (split_pos==-1){
			return nmemb;
		}
		int line_len = size * nmemb;
		char szKey[128] = {0};
		memcpy(szKey, buffer, split_pos);
		StringUtils::TrimStart(szKey, " ");
		StringUtils::TrimEnd(szKey, " ");
		if (ALICE_STRICMP(szKey, "Set-Cookie") == 0) {
			if (line_len - split_pos - 1>4095){
				http->mServerCookie.Set((char*)buffer + split_pos + 1, 4095);
			}
			else {
				http->mServerCookie.Set((char*)buffer + split_pos + 1, line_len - split_pos - 1);
			}
		}
		return nmemb;
	}
	size_t HttpCurl::OnReceiveBodyData(void *buffer, size_t size, size_t nmemb, void *lpVoid){
		HttpCurl*http = (HttpCurl*)lpVoid;
		memcpy(http->mResponseData.mText + http->mResponseData.mLen, buffer, size*nmemb);
		http->mResponseData.mLen += size * nmemb;
		return nmemb;
	}
	static void dump(const char *text,FILE *stream, unsigned char *ptr, size_t size){
		size_t i;
		size_t c;
		unsigned int width = 0x10;
		fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
			text, (long)size, (long)size);

		for (i = 0; i < size; i += width) {
			fprintf(stream, "%4.4lx: ", (long)i);
			for (c = 0; c < width; c++) {
				if (i + c < size)
					fprintf(stream, "%02x ", ptr[i + c]);
				else
					fputs("   ", stream);
			}
			for (c = 0; (c < width) && (i + c < size); c++) {
				char x = (ptr[i + c] >= 0x20 && ptr[i + c] < 0x80) ? ptr[i + c] : '.';
				fputc(x, stream);
			}
			fputc('\n', stream);
		}
	}
	int HttpCurl::OnDebugFunction(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
		return 0;
		const char *text;
		switch (type) {
		case CURLINFO_TEXT:
			fprintf(stderr, "== Info: %s", data);
		default:
			return 0;

		case CURLINFO_HEADER_OUT:
			text = "=> Send header";
			break;
		case CURLINFO_DATA_OUT:
			text = "=> Send data";
			break;
		case CURLINFO_SSL_DATA_OUT:
			text = "=> Send SSL data";
			break;
		case CURLINFO_HEADER_IN:
			text = "<= Recv header";
			break;
		case CURLINFO_DATA_IN:
			text = "<= Recv data";
			break;
		case CURLINFO_SSL_DATA_IN:
			text = "<= Recv SSL data";
			break;
		}
		dump(text, stderr, (unsigned char *)data, size);
		return 0;
	}
}
