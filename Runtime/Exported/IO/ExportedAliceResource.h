#pragma once
#include "Runtime/IO/AliceResource.h"

//2016 08/24 2.8.7 pass 

namespace Alice
{
	class ExportedAliceResource :public Resource
	{
	public:
		DEFINE_LUA_API(GetData);
		DEFINE_LUA_API(GetType);
		DEFINE_LUA_API(Unload);
		DEFINE_LUA_API(Add);
		DEFINE_LUA_API(Remove);
		DEFINE_LUA_API(Load);
		DEFINE_LUA_API(LoadByte);
		DEFINE_LUA_API(GetRes);
		DEFINE_LUA_API(Dump);
		DEFINE_LUA_API(UnloadUnusedResources);
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedAliceResource, Resource)
	};
}
