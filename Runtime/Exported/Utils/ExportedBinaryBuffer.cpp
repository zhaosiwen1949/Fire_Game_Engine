#include "ExportedBinaryBuffer.h"
namespace Alice {
	int ExportedBinaryBuffer::API_AppendInt(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int int_value = lua_tointeger(L, 2);
		ptr->AppendInt(int_value);
		return 0;
	}
	int ExportedBinaryBuffer::API_GetInt(lua_State*L){
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int offset = lua_tointeger(L, 2);
		lua_pushinteger(L, *((int*)(ptr->mBuffer+offset)));
		return 1;
	}
	int ExportedBinaryBuffer::API_GetShort(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int offset = lua_tointeger(L, 2);
		lua_pushinteger(L, *((short*)(ptr->mBuffer + offset)));
		return 1;
	}
	int ExportedBinaryBuffer::API_GetChar(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int offset = lua_tointeger(L, 2);
		lua_pushinteger(L, *((char*)(ptr->mBuffer + offset)));
		return 1;
	}
	int ExportedBinaryBuffer::API_GetFloat(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int offset = lua_tointeger(L, 2);
		lua_pushnumber(L, *((float*)(ptr->mBuffer + offset)));
		return 1;
	}
	int ExportedBinaryBuffer::API_GetData(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int offset = lua_tointeger(L, 2);
		int count = lua_tointeger(L, 3);
		lua_pushlstring(L, (char*)(ptr->mBuffer + offset), count);
		return 1;
	}

	int ExportedBinaryBuffer::API_Append(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		const char* data = lua_tostring(L, 2);
		int count = lua_tointeger(L, 3);
		ptr->Append(data,count);
		return 0;
	}

	int ExportedBinaryBuffer::API_Shift(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int count = lua_tointeger(L, 2);
		ptr->Shift(count);
		return 0;
	}
	int ExportedBinaryBuffer::API_GetDataSize(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		lua_pushinteger(L, ptr->mDataSize);
		return 1;
	}
	int ExportedBinaryBuffer::API_TrimEnd(lua_State*L) {
		BinaryBuffer*ptr = TO_USERDATA(L, BinaryBuffer, 1);
		int count = lua_tointeger(L, 2);
		ptr->TrimEnd(count);
		return 0;
	}
	int ExportedBinaryBuffer::Export(lua_State*L){
		luaL_Reg apis[] ={
			EXPORT_API(GetInt)
			EXPORT_API(GetShort)
			EXPORT_API(GetChar)
			EXPORT_API(GetFloat)
			EXPORT_API(GetData)
			EXPORT_API(GetDataSize)
			EXPORT_API(AppendInt)
			EXPORT_API(Append)
			EXPORT_API(Shift)
			EXPORT_API(TrimEnd)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, BinaryBuffer, apis);
		return 0;
	}
}