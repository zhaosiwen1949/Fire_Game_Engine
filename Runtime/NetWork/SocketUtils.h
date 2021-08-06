#pragma once
/*
������Ҫ�����ڿ�ƽ̨��Socket�궨��
*/
#include "Runtime/RuntimePrefix.h"
#ifdef ALICE_PLATFORM_WIN
#define AliceSocket SOCKET
#define AliceFDSet fd_set
#define AliceAddressInfo addrinfo
#define NewSocket socket
#define AliceCloseSocket closesocket
#elif ALICE_PLATFORM_UNIX
#define NewSocket socket
#define AliceFDSet fd_set
#define AliceAddressInfo addrinfo
#define AliceSocket int
#define AliceCloseSocket close
#endif
namespace Alice {
	int GetHostFromURL(const char *url, int len, char *host);
	int GetPortFromHost(const char *host, int len);
	void TrimPortFromHost(char *host, int len);
	extern int gConnectTimeOut;
	bool CanRead(AliceSocket fd, int second = 0, int ms = 0);
	bool CanWrite(AliceSocket fd, int second = 0, int ms = 0);
	AliceSocket ConnectWithSpecificProtocol(const char *peer, int port,int port6, int protocol, bool confirm_connection);
	AliceSocket ConnectSocket(const char *peer, int port, int portv6, bool confirm_connection);
	//�����첽���ӵ�ʱ�򣬲��ܱ�֤�����ӳɹ�������ָ��Э��汾
	AliceSocket AsyncConnectSocket(const char *peer, int port, int portv6,int protocol=AF_UNSPEC);
	//���һ��������ʾ���Ƿ�֤�����ӳɹ����������⣬����ҪӦ�ò�����֤�������Ѿ��ɹ�
	//���һ����������true�������������socket����첽�ķ�ʽ��ʹ�����ӵ�ʱ�򲻻Ῠ��
	AliceSocket ConnectPeer(AliceAddressInfo*address_info, int port, int port6, bool confirm_connection);
	void MakeSocketUnblock(AliceSocket fd);
	void MakeSocketBlock(AliceSocket fd);
	bool GetIPAddress(const char* domain_name, int ipversion, char * ip);
}