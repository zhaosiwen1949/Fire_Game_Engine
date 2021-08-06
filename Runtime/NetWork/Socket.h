#pragma once
#include "SocketUtils.h"
#include "Runtime/Base/Object.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/String/BinaryBuffer.h"
namespace Alice {
	class Socket : public Object {
	public:
		Socket();
		~Socket();
		AliceSocket mSocket;
		BinaryBuffer mSendBuffer,mRecvBuffer;
		FixedString mPeerIP;
		int mBufferSize;
		int mPort,mPort6;
		bool mbConnected;
    public:
		//初始化Socket对象，该socket对象可以有缓冲区，也可以不要缓冲区，最后一个buffersize是0的话，就没有缓冲区
		void Init(const char * ip, int port,int port6, int bufferSize=0);
		bool Connect();
		bool AsyncConnect(int protocol);
		int Connected();
		int Close();
		int Recv();
		int DoRecv();
		int Send();
		int DoSend();
		//新版本的接收和发送数据接口
		//传入参数为期望发送或者接受到的数据的量，
		//-1表示接收所有的数据，直到缓冲区满为止，
		//0表示接收一次数据，无论数据量是多少都可以
		//正数表示期待接收或者发送的数据量，如果没有达到期望的数据量，则会循环发送，直到数据发送或者接收完毕才返回
		int ReceiveData(int target_size,unsigned char*buffer,int len);
		int SendData(int target_size, unsigned char*buffer, int len);
	protected:
		bool IsPeerClosed();
	public:
		static void CleanUp();
		static int DefaultGC(lua_State*L);
	public:
		DECLEAR_ALICE_CLASS(Socket);
	};
}
