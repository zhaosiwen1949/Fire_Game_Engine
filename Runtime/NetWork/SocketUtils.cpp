#include "SocketUtils.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/String/StringUtils.h"
#ifdef ALICE_PLATFORM_WIN
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#endif
namespace Alice {
	int gConnectTimeOut = 5;
	int GetHostFromURL(const char *url, int len, char *host) {
		for (int i = 0; i < len; ++i) {
			if (url[i] == '/') {
				memcpy(host, url, i);
				return i;
			}
		}
		return len;
	}
	int GetPortFromHost(const char *host, int len) {
		for (int i = 0; i < len; ++i) {
			if (host[i] == ':') {
				char szBuffer[32] = { 0 };
				memcpy(szBuffer, host + i + 1, len - i - 1);
				return atoi(szBuffer);
			}
		}
		return 80;
	}
	void TrimPortFromHost(char *host, int len) {
		for (int i = 0; i < len; ++i) {
			if (host[i] == ':') {
				memset(host + i, 0, len - i);
				return;
			}
		}
	}
	bool CanRead(AliceSocket fd, int second, int ms) {
		AliceFDSet fdset;
		memset(&fdset, 0, sizeof(AliceFDSet));
		timeval timeout = { second,ms };
#ifdef ALICE_PLATFORM_WIN
		fdset.fd_count = 1;
		fdset.fd_array[0] = fd;
		int nCount = select(0, &fdset, NULL, NULL, &timeout);
		return nCount == 1;
#elif ALICE_PLATFORM_UNIX
		FD_SET(fd, &fdset);
		if (select(fd + 1, NULL, &fdset, NULL, &timeout) <= 0) {
			return false;
		}
		if (FD_ISSET(fd, &fdset)) {
			return true;
		}
		return false;
#endif
	}
	bool CanWrite(AliceSocket fd, int second, int ms) {
		AliceFDSet fdset;
		memset(&fdset, 0, sizeof(AliceFDSet));
		timeval timeout = { second,ms };
#ifdef ALICE_PLATFORM_WIN
		fdset.fd_count = 1;
		fdset.fd_array[0] = fd;
		int nCount = select(0, NULL, &fdset, NULL, &timeout);
		return nCount == 1;
#elif ALICE_PLATFORM_UNIX
		FD_SET(fd, &fdset);
		if (select(fd + 1, NULL, &fdset, NULL, &timeout) <= 0) {
			return false;
		}
		if (FD_ISSET(fd, &fdset)) {
			return true;
		}
		return false;
#endif
	}
	AliceSocket ConnectWithSpecificProtocol(const char *peer, int port, int port6, int protocol, bool confirm_connection) {
		AliceAddressInfo hints, *res, *aip;
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = protocol;
		int error = getaddrinfo(peer, NULL, &hints, &res);
		if (error == 0) {
			AliceSocket fd = 0;
			for (aip = res; aip; aip = aip->ai_next) {
				fd = ConnectPeer(aip, port,port6,confirm_connection);
				if (fd != 0) {
					break;
				}
			}
			freeaddrinfo(res);
			return fd;
		}
		return 0;
	}
	AliceSocket ConnectSocket(const char *peer, int port, int portv6, bool confirm_connection) {
		AliceSocket fd = ConnectWithSpecificProtocol(peer,port,portv6, AF_INET, confirm_connection);
		if (fd != 0) {
			return fd;
		}
		fd = ConnectWithSpecificProtocol(peer, port,portv6,AF_INET6, confirm_connection);
		return fd;
	}
	AliceSocket AsyncConnectSocket(const char *peer, int port, int portv6, int protocol) {
		return ConnectWithSpecificProtocol(peer, port, portv6, protocol, false);
	}
	AliceSocket ConnectPeer(AliceAddressInfo*address_info, int port, int port6, bool confirm_connection) {
		AliceSocket sockfd = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
		if (sockfd < 0) {
			return 0;
		}
		if (address_info->ai_family == AF_INET6) {
            MakeSocketUnblock(sockfd);
            char szIPAddress[256]={0};
            inet_ntop(AF_INET6,&(((sockaddr_in6*)address_info->ai_addr)->sin6_addr),  szIPAddress, INET6_ADDRSTRLEN);
			((sockaddr_in6*)(address_info->ai_addr))->sin6_port = htons(port6);
		}
		else if (address_info->ai_family == AF_INET) {
            MakeSocketUnblock(sockfd);
			((sockaddr_in*)(address_info->ai_addr))->sin_port = htons(port);
		}
		else {
			AliceCloseSocket(sockfd);
			return 0;
		}
		if (connect(sockfd, address_info->ai_addr, address_info->ai_addrlen) == -1) {
#ifdef ALICE_PLATFORM_WIN
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
#elif ALICE_PLATFORM_UNIX
			if (errno != EINPROGRESS) {
#endif
				Error("socket connect fail");
				AliceCloseSocket(sockfd);
				return 0;
			}
		}
		if (confirm_connection){
			int nTimes = 0;
			while (nTimes < gConnectTimeOut) {
				if (CanWrite(sockfd, 1)) {
					MakeSocketBlock(sockfd);
					return sockfd;
				}
				nTimes++;
			}
			AliceCloseSocket(sockfd);
			return 0;
		}
		return sockfd;
	}
	void MakeSocketBlock(AliceSocket fd) {
#ifdef ALICE_PLATFORM_WIN
		unsigned long ul = 0;
		ioctlsocket(fd, FIONBIO, (unsigned long *)&ul);
#elif ALICE_PLATFORM_UNIX
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags &~O_NONBLOCK);
#endif
	}
	void MakeSocketUnblock(AliceSocket fd) {
#ifdef ALICE_PLATFORM_WIN
		unsigned long ul = 1;
		ioctlsocket(fd, FIONBIO, (unsigned long *)&ul);
#elif ALICE_PLATFORM_UNIX
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
	}

	bool GetIPAddress(const char* domain_name, int ipversion, char * ip) {
		addrinfo hints, *res, *aip;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = ipversion;
		hints.ai_socktype = SOCK_STREAM;
		int error = getaddrinfo(domain_name, NULL, &hints, &res);
		if (error != 0) {
			return false;
		}
		int sockfd = -1;
		for (aip = res; aip; aip = aip->ai_next) {
			if (aip->ai_family == AF_INET6 && ipversion == AF_INET6) {
#ifdef ALICE_PLATFORM_WIN
				TCHAR ipv6[128] = { 0 };
				DWORD ipbufferlength = 128;
				if (0 == WSAAddressToString((LPSOCKADDR)aip->ai_addr, (DWORD)aip->ai_addrlen, NULL, ipv6, &ipbufferlength)) {
					freeaddrinfo(res);
					Alice::StringUtils::UnicodeToASCII(ipv6, ip, 128);
					return true;
				}
#elif ALICE_PLATFORM_UNIX
                if (inet_ntop(AF_INET6, &(((sockaddr_in6*)aip->ai_addr)->sin6_addr), ip, INET6_ADDRSTRLEN) != NULL) {
                    freeaddrinfo(res);
                    return true;
                }
#endif
				freeaddrinfo(res);
				return false;
			}
			else if (aip->ai_family == AF_INET && ipversion == AF_INET) {
#ifdef APLICE_PLATFORM_WIN
				TCHAR ipv4[128] = { 0 };
				DWORD ipbufferlength = 128;
				if (0 == WSAAddressToString((LPSOCKADDR)aip->ai_addr, (DWORD)aip->ai_addrlen, NULL, ipv4, &ipbufferlength)) {
					freeaddrinfo(res);
					Alice::StringUtils::UnicodeToASCII(ipv4, ip, 128);
					return true;
				}
#elif ALICE_PLATFORM_UNIX
                strcpy(ip, inet_ntoa(((sockaddr_in*)aip->ai_addr)->sin_addr));
#endif
				freeaddrinfo(res);
				return false;
			}
		}
		freeaddrinfo(res);
		return false;
	}
}
