#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/NetWork/Socket.h"
namespace Alice{
	class ExportedSocket : public Socket{
	public:
		DEFINE_LUA_API(Init);
		DEFINE_LUA_API(Connect);
		DEFINE_LUA_API(AsyncConnect);
		DEFINE_LUA_API(Connected);
		DEFINE_LUA_API(Close);
		DEFINE_LUA_API(Recv);
		DEFINE_LUA_API(Send);
		DEFINE_LUA_API(GetSendBuffer);
		DEFINE_LUA_API(GetRecvBuffer);
		DEFINE_LUA_API(Release);
		static int Export(lua_State*L);
	public:
		DEFINE_LUA_API(GetIPAddress);
		DEFINE_LUA_API(SetTimeOut);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedSocket, Socket);
	};
}
