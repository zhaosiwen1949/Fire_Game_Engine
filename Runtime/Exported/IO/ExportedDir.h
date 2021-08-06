#pragma once
#include "Runtime/Base/Object.h"
namespace Alice{
	class ExportedDir{
	public:
		DEFINE_LUA_API(Exist);
		DEFINE_LUA_API(Create);
		DEFINE_LUA_API(Delete);
		DEFINE_LUA_API(GetItemList);
		static int Export(lua_State*L);
	};
}
