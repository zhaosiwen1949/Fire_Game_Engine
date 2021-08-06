#pragma once
/*
这里主要是用于跨平台的Socket宏定义
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
	//进行异步链接的时候，不能保证能链接成功，所以指定协议版本
	AliceSocket AsyncConnectSocket(const char *peer, int port, int portv6,int protocol=AF_UNSPEC);
	//最后一个参数表示，是否保证该链接成功，如果不检测，则需要应用层来保证该链接已经成功
	//最后一个参数传入true，则可以让整个socket变成异步的方式，使得链接的时候不会卡顿
	AliceSocket ConnectPeer(AliceAddressInfo*address_info, int port, int port6, bool confirm_connection);
	void MakeSocketUnblock(AliceSocket fd);
	void MakeSocketBlock(AliceSocket fd);
	bool GetIPAddress(const char* domain_name, int ipversion, char * ip);
}