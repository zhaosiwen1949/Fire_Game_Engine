#include "Socket.h"
#ifdef ALICE_PLATFORM_WIN
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#endif
#include "Runtime/String/StringUtils.h"
#include "Runtime/Debugger/Log.h"
namespace Alice {
	static std::set<Socket*> sSockets;
	void Socket::CleanUp() {
		for (auto iter=sSockets.begin();iter!=sSockets.end();++iter){
			(*iter)->Close();
		}
		sSockets.clear();
	}
    Socket::Socket() {
        mPeerIP.Resize(64);
		mSocket = 0;
		sSockets.insert(this);
    }
	Socket::~Socket() {
		auto iter = sSockets.find(this);
		if (iter!=sSockets.end()){
			sSockets.erase(iter);
		}
		Close();
	}
	void Socket::Init(const char * ip, int port, int port6, int bufferSize) {
		mPeerIP = ip;
		mPort = port;
		mPort6 = port6;
		mBufferSize = bufferSize;
		if (mBufferSize > 0) {
			mRecvBuffer.SetSize(mBufferSize);
			mSendBuffer.SetSize(mBufferSize);
		}
	}
	bool Socket::Connect() {
		if (mSocket!=0){
			AliceCloseSocket(mSocket);
			mSocket = 0;
		}
		mbConnected = false;
		mSocket = ConnectSocket(mPeerIP.mText, mPort, mPort6, true);
		return mSocket!=0;
	}
	bool Socket::AsyncConnect(int protocol) {
		if (mSocket != 0) {
			AliceCloseSocket(mSocket);
			mSocket = 0;
		}
		mbConnected = false;
		mSocket = AsyncConnectSocket(mPeerIP.mText, mPort, mPort6, protocol);
		return mSocket!=0;
	}
	int Socket::Connected() {
		if (CanWrite(mSocket)){
			mbConnected = true;//连上了
			return 1;
		}
		return 0;//不是断线也不是没连上，待观察
	}
	int Socket::Close() {
		if (mSocket==0){
			return 0;
		}
		int ret=AliceCloseSocket(mSocket);
		mSocket = 0;
		mbConnected = false;
		return ret;
	}
	int Socket::Recv() {
		if (mbConnected==false){
			return -1;
		}
		if (CanRead(mSocket)){
			return DoRecv();
		}
		return 0;
	}
	int Socket::Send() {
		if (mbConnected == false) {
			return -1;
		}
		if (CanWrite(mSocket)){
			return DoSend();
		}
		return 0;
	}
	int Socket::SendData(int target_size/* =0 */, unsigned char*buffer, int len) {
		if (len <= 0) {
			Error("send buffer is invalid %d", len);
			return 0;
		}
		if (target_size > len) {
			Error("Socket::SendData target size %d is bigger than buffer %d", target_size, len);
			return 0;
		}
		int sent_size = 0;
		while (CanWrite(mSocket)){
			int nLen = send(mSocket, (char*)buffer, len, 0);
#ifdef ALICE_PLATFORM_WIN
			if (nLen<=0){
				return -1;//产生了错误
			}
#elif ALICE_PLATFORM_UNIX
            if(nLen<0){
                if((errno!=EAGAIN)&&(errno!=EWOULDBLOCK)){
                    break;
                }
            }
#endif
			sent_size += nLen;
			len -= nLen;
			if (target_size==-1){
				if (len==0){
					break;
				}
			}
			else if (target_size == 0) {
				break;
			}
			else if (target_size > 0) {
				if (target_size==sent_size){
					break;
				}
			}
		}
		return sent_size;
	}
	int Socket::ReceiveData(int target_size/* =0 */, unsigned char*buffer, int len) {
		if (len <= 0) {
			Error("receive buffer is invalid %d", len);
			return 0;
		}
		if (target_size>len){
			Error("Socket::ReceiveData target size %d is bigger than buffer %d",target_size,len);
			return 0;
		}
		int read_size = 0;
		while (len>0) {
			if (CanRead(mSocket)==false){
				continue;
			}
			int nLen = recv(mSocket, (char*)buffer + read_size, len, 0);
			if (nLen == 0) {
				return read_size;
			}
			if (nLen < 0) {
				if (IsPeerClosed()) {
					return read_size;
				}
			}
			read_size += nLen;
			len -= nLen;
			if (target_size == -1) {
				if (len == 0) {
					break;
				}
			}
			else if (target_size == 0) {
				break;
			}
			else if (target_size > 0) {
				if (target_size == read_size) {
					break;
				}
			}
		}
		return read_size;
	}

	int Socket::DoSend() {
		if (mSendBuffer.mDataSize > 0){
			int nLen = send(mSocket, (char*)mSendBuffer.mBuffer, mSendBuffer.mDataSize, 0);
			if (nLen > 0) {
				mSendBuffer.Shift(nLen);
			}
			return nLen;//返回发送出去的数据大小
		}
        return mSendBuffer.mDataSize;
	}
#ifdef ALICE_PLATFORM_WIN
	bool Socket::IsPeerClosed() {
		int err = WSAGetLastError();
		switch (err) {
		case WSAENOTCONN:
			return true;
		case WSAECONNABORTED:
			return true;
		case WSAECONNRESET:
			return true;
		default:
			Debug("unkown error %d", err);
			break;
		}
		return false;
	}
#elif ALICE_PLATFORM_UNIX
	bool Socket::IsPeerClosed() {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			return true;
		}
		return false;
	}
#endif
	int Socket::DoRecv() {
		int nLen = recv(mSocket, (char*)mRecvBuffer.mBuffer + mRecvBuffer.mDataSize, mBufferSize - mRecvBuffer.mDataSize, 0);
		if (nLen == 0) {
			Close();
			return -1;
		}
		if (nLen<0){
			if (IsPeerClosed()) {
				Close();
				return -1;
			}
			return mRecvBuffer.mDataSize;
		}
		//Debug("recev data %d bytes,current cache buffer %d",nLen,mRecvBuffer.mDataSize);
        mRecvBuffer.mDataSize += nLen;
        return mRecvBuffer.mDataSize;
	}
	int Socket::DefaultGC(lua_State*L) {
		Socket*userData = TO_USERDATA(L, Socket, 1);
		delete userData;
		return 0;
	}
}
