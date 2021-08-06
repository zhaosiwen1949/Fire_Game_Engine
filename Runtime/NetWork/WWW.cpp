#include "Runtime/NetWork/WWW.h"
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Thread/MainThreadScheduler.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/IO/FileSystem.h"
extern "C" {
#include "Runtime/Plugins/md5/md5.h"
}
#include "Runtime/Plugins/base64.h"
#if ALICE_PLATFORM_UNIX
#include "unistd.h"
#define _access access
#elif ALICE_PLATFORM_WIN
#include "Socket.h"
#include <io.h>
#endif
#include <thread>
#include <math.h>
#include "Runtime/Debugger/Log.h"
#if ALICE_PLATFORM_WIN
#pragma comment(lib,"libcurl.lib")
#endif
namespace Alice{
	WWW::WWW() : timeout(10){
		mFileMD5.Resize(64);
		mUrl.Resize(1024);
		mLocalFilePath.Resize(1024);
		mServerName.Resize(128);
		mProgress = 0.0;
		mTotalFileSize = 0;
		mDownloadedFileSize = 0;
		mPreferedIPVersion = CURL_IPRESOLVE_WHATEVER;
	}
	WWW::~WWW() {
		Debug("delete www %p %d",this,mState);
	}
	void WWW::Body() {
		mSendBufferSize = 0;
		mReceiveBufferSize = 0;
		if (GetTargetFileSize() <= 0) {
			mState = kThreadStateEnd;
			mErrorCode = CURLE_REMOTE_FILE_NOT_FOUND;
			return;
		}
		//Debug("www target file size %d state %d",mTotalFileSize,mState);
		mDownloadedFileSize = FileSystem::FileSizeOf(mLocalFilePath.mText);
		if (IsDownloadSucessed()){
			mState = kThreadStateEnd;
			mErrorCode = CURLcode::CURLE_OK;
			return;
		}
		else {
			mDownloadedFileSize = 0;
			Alice::ResourceManager::DeleteFileWithPath(mLocalFilePath.mText);
		}
		mErrorCode = DownloadFile();
		if (false==IsDownloadSucessed()) {
			mDownloadedFileSize = 0;
			Alice::ResourceManager::DeleteFileWithPath(mLocalFilePath.mText);
			mErrorCode = -1;//下载失败
		}
		mState = kThreadStateEnd;
	}
	void WWW::Update() {
		if (mState==kThreadStateBusy){
			if (mListener!=nullptr){
				mListener->OnProgress(this);
			}
		}
		else if (mState == kThreadStateEnd) {
			mState = kThreadStateCreated;
			if (mListener != nullptr) {
				if (mErrorCode==CURLcode::CURLE_OK){
					mListener->OnSucess(this);
				} else {
					mListener->OnError(this);
				}
				if (mState == kThreadStateRelease) {
					return;
				}
			}
		}
	}
	int WWW::Processing(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded){
		Alice::WWW* WWW = (Alice::WWW*)ptr;
		if (!WWW || WWW->mTotalFileSize == 0 || nowDownloaded == 0) return 0;
		double nowDown = (WWW->mTotalFileSize - totalToDownload + nowDownloaded);
		double curpercent = nowDown / WWW->mTotalFileSize * 100.0;
		WWW->mProgress=curpercent;
		return 0;
	}
	size_t WWW::OnReceivedHeaderData(void *buffer, size_t size, size_t nmemb, void *lpVoid){
		WWW*www = (WWW*)lpVoid;
		int line_len = size * nmemb;
		char *data = (char*)buffer;
		for (int i = 0; i < line_len; ++i) {
			if (data[i] == ':') {
				if (memcmp("Content-MD5",data,i)==0){
					if (www->mFileMD5.mLen == 0) {
						char header_property_value_base64_encoded[36] = { 0 };
						memcpy(header_property_value_base64_encoded, data + i + 2, line_len - i - 4);
						if (line_len - i - 4==32){
							www->mFileMD5 = header_property_value_base64_encoded;
						}
						else {
							char md5value[36] = { 0 };
							std::string header_property_value = base64_decode(header_property_value_base64_encoded, line_len - i - 4);
							md5bytes2hexa((const unsigned char*)header_property_value.c_str(), md5value);
							www->mFileMD5 = md5value;
						}
					}
				}
			}
		}
		return nmemb;
	}
	size_t WWW::OnReceiveData(void *buffer, size_t size, size_t nmemb, void *userdata){
		WWW*www = (WWW*)userdata;
		char szTempFilePath[1024] = { 0 };
		strcpy(szTempFilePath, www->mLocalFilePath.mText);
		strcat(szTempFilePath, ".temp");
		FILE*fp = fopen(szTempFilePath, "ab");
		if (fp){
			www->mDownloadedFileSize += size * nmemb;
			fwrite(buffer, size, nmemb, fp);
			fclose(fp);
		}
		return nmemb;
	}
	bool WWW::IsDownloadSucessed() {
		if (mDownloadedFileSize==mTotalFileSize){
			if (mFileMD5.mLen>0){//有MD5就比较一下MD5，看看是否正确，如果MD5值一样，那么就说明已经下载完了，不需要重复下载
				Data file_data;
				if (FileSystem::LoadDataFromPath(mLocalFilePath.mText,file_data)){
					char md5value[64] = { 0 };
					md5hexa((char*)file_data.mData, file_data.mDataLen, md5value);
					if (mFileMD5 == md5value) {//这种情况下，表示已经下载成功了
						return true;
					}
				}
			}
		}
		return false;
	}
	AliceSInt64 WWW::GetTargetFileSize() {
		CURL *curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, mUrl.mText);
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, mPreferedIPVersion);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnReceivedHeaderData);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		CURLcode ret = curl_easy_perform(curl);
		if (ret == CURLE_OK) {
#if ALICE_ANDROID
			double total_file_size = 0.0;
			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &total_file_size);
			mTotalFileSize = int(total_file_size);
#else
			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &mTotalFileSize);
#endif
		} else {
			mTotalFileSize = 0;
			Error("WWW get target file size failed %d", ret);
		}
		long send_data_size;
		long receive_data_size;
		curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &send_data_size);
		curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &receive_data_size);
		mSendBufferSize += send_data_size;
		mReceiveBufferSize += receive_data_size;
		//Debug("WWW GetTargetFileSize info : %d %d", mSendBufferSize, mReceiveBufferSize);
		curl_easy_cleanup(curl);
		return mTotalFileSize;
	}
	CURLcode WWW::DownloadFile() {
		CURL *curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, mUrl.mText);
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, mPreferedIPVersion);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnReceivedHeaderData);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WWW::OnReceiveData);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, WWW::Processing);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		//curl_easy_setopt(curl, CURLOPT_RESUME_FROM, mDownloadedFileSize);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, -1);
		if (mCookie.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_COOKIE, mCookie.mText);
		}
		if (mReferer.mLen > 0) {
			curl_easy_setopt(curl, CURLOPT_REFERER, mReferer.mText);
		}
		CURLcode res = curl_easy_perform(curl);
		if (res==CURLcode::CURLE_OK){
			Data temp;
			char szTempFilePath[1024] = { 0 };
			strcpy(szTempFilePath, mLocalFilePath.mText);
			strcat(szTempFilePath, ".temp");
			if (FileSystem::LoadDataFromPath(szTempFilePath, temp)) {
				FileSystem::SaveData(mLocalFilePath.mText, temp);
				ResourceManager::DeleteFileWithPath(szTempFilePath);
			}
		}
		long send_data_size,receive_header_size;
		curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &send_data_size);
		curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &receive_header_size);
#if ALICE_ANDROID
		double receive_data_size_x = 0.0f;
		curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &receive_data_size_x);
		int receive_data_size = int(receive_data_size_x);
#else
		curl_off_t receive_data_size = 0;
		curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &receive_data_size);
#endif
		mSendBufferSize += send_data_size;
		mReceiveBufferSize += receive_header_size;
		mReceiveBufferSize += receive_data_size;
		//Debug("WWW download info : %d %d",mSendBufferSize,mReceiveBufferSize);
        curl_easy_cleanup(curl);
		return res;
	}
}
