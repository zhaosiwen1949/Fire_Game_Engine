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
		//��ʼ��Socket���󣬸�socket��������л�������Ҳ���Բ�Ҫ�����������һ��buffersize��0�Ļ�����û�л�����
		void Init(const char * ip, int port,int port6, int bufferSize=0);
		bool Connect();
		bool AsyncConnect(int protocol);
		int Connected();
		int Close();
		int Recv();
		int DoRecv();
		int Send();
		int DoSend();
		//�°汾�Ľ��պͷ������ݽӿ�
		//�������Ϊ�������ͻ��߽��ܵ������ݵ�����
		//-1��ʾ�������е����ݣ�ֱ����������Ϊֹ��
		//0��ʾ����һ�����ݣ������������Ƕ��ٶ�����
		//������ʾ�ڴ����ջ��߷��͵������������û�дﵽ�����������������ѭ�����ͣ�ֱ�����ݷ��ͻ��߽�����ϲŷ���
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
