#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/String/BinaryBuffer.h"
namespace Alice{
	class ExportedBinaryBuffer : public BinaryBuffer{
	public:
		DEFINE_LUA_API(AppendInt);
		DEFINE_LUA_API(GetInt);
		DEFINE_LUA_API(GetShort);
		DEFINE_LUA_API(GetChar);
		DEFINE_LUA_API(GetFloat);
		DEFINE_LUA_API(GetData);
		DEFINE_LUA_API(GetDataSize);
		DEFINE_LUA_API(Append);
		DEFINE_LUA_API(Shift);
		DEFINE_LUA_API(TrimEnd);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedBinaryBuffer,BinaryBuffer)
	};
}
