/*
* lbase64.c
* base64 encoding and decoding for Lua 5.1
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 23 Mar 2010 22:22:38
* This code is hereby placed in the public domain.
*/

#include <string.h>
#include "Runtime/Plugins/Lua/lua.hpp"
#include "base64.h"
#include "Runtime/Debugger/log.h"

int Lencode(lua_State*L) {
	const char*src = lua_tostring(L,1);
	std::string base64_encoded=base64_encode((unsigned char*)src, strlen(src));
	lua_pushlstring(L, base64_encoded.c_str(), base64_encoded.size());
	//Info("Lencode src %d dst %d",strlen(src),strlen(szBuffer));
	return 1;
}

int Ldecode(lua_State*L) {
	const char*src = lua_tostring(L, 1);
	std::string base64_decoded=base64_decode(src, strlen(src));
	lua_pushlstring(L, base64_decoded.c_str(), base64_decoded.size());
	//Info("Ldecode src %d dst %d", strlen(src), strlen(szBuffer));
	return 1;
}

static const luaL_Reg R[] =
{
	{ "encode",	Lencode	},
	{ "decode",	Ldecode	},
	{ NULL,		NULL	}
};

extern "C" int luaopen_base64(lua_State *L)
{
	luaL_register(L, "base64", R);
	return 1;
}
