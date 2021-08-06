#include "ExportedSocket.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Exported/Utils/ExportedBinaryBuffer.h"
namespace Alice{
	int ExportedSocket::API_Init(lua_State*L){
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		int port6 = 9527;
		int bufferSize = 20480;
		if (lua_gettop(L) >= 4) {
			port6 = lua_tointeger(L, 4);
			if (lua_gettop(L) >= 5){
				bufferSize = lua_tointeger(L, 5);
			}
		}
		ptr->Init(lua_tostring(L, 2), lua_tointeger(L, 3), port6, bufferSize);
		if (bufferSize!=0){
			BinaryBuffer*recvBuffer = &ptr->mRecvBuffer;
			BinaryBuffer*sendBuffer = &ptr->mSendBuffer;
			CreateLuaDataViaExistObject(recvBuffer, BinaryBuffer, L);
			CreateLuaDataViaExistObject(sendBuffer, BinaryBuffer, L);
			recvBuffer->PushUserData(L);
			sendBuffer->PushUserData(L);
			return 2;
		}
		return 0;
	}
	int ExportedSocket::API_Connect(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		lua_pushboolean(L, ptr->Connect());
		return 1;
	}
	int ExportedSocket::API_AsyncConnect(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		int protocol = AF_UNSPEC;
		if (lua_gettop(L)>1){
			int ver = lua_tointeger(L, 2);
			if (ver==4){
				protocol = AF_INET;
			}else if (ver==6){
				protocol = AF_INET6;
			}
		}
		lua_pushboolean(L, ptr->AsyncConnect(protocol));
		return 1;
	}
	int ExportedSocket::API_Connected(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		lua_pushinteger(L, ptr->Connected());
		return 1;
	}
	int ExportedSocket::API_Close(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		lua_pushinteger(L, ptr->Close());
		return 1;
	}

	int ExportedSocket::API_Recv(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		lua_pushinteger(L, ptr->Recv());
		return 1;
	}

	int ExportedSocket::API_Send(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		lua_pushinteger(L, ptr->Send());
		return 1;
	}

	int ExportedSocket::API_GetSendBuffer(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		ptr->mSendBuffer.PushUserData(L);
		return 1;
	}

	int ExportedSocket::API_GetRecvBuffer(lua_State*L) {
		Socket*ptr = TO_USERDATA(L, Socket, 1);
		ptr->mRecvBuffer.PushUserData(L);
		return 1;
	}
	int ExportedSocket::API_Release(lua_State*L){
		ExportedSocket*ptr = TO_USERDATA(L, ExportedSocket, -1);
		luaL_unref(L, LUA_REGISTRYINDEX, ptr->mLuaData);
		ptr->mLuaData = LUA_REFNIL;
		return 0;
	}
	int ExportedSocket::Export(lua_State*L){
		luaL_Reg apis[] = {
			EXPORT_API(Init)
			EXPORT_API(Connect)
			EXPORT_API(AsyncConnect)
			EXPORT_API(Connected)
			EXPORT_API(Close)
			EXPORT_API(Recv)
			EXPORT_API(Send)
			EXPORT_API(GetSendBuffer)
			EXPORT_API(GetRecvBuffer)
			EXPORT_API(Release)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, Socket, apis);
		luaL_Reg api_c[] = {
			{ "New",New_ExportedAliceType },
			EXPORT_API(GetIPAddress)
			EXPORT_API(SetTimeOut)
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Socket, api_c);
		return 0;
	}
	int ExportedSocket::API_GetIPAddress(lua_State*L) {
		int ipversion = lua_tointeger(L, 2);
		char ip[128] = {0};
		if (GetIPAddress( lua_tostring(L, 1), ipversion == 4 ? AF_INET : AF_INET6, ip)){
			lua_pushstring(L, ip);
		}else {
			lua_pushstring(L,"unkown");
		}
		return 1;
	}
	int ExportedSocket::API_SetTimeOut(lua_State*L) {
		gConnectTimeOut = lua_tointeger(L, 1);
		return 0;
	}
}
