#include "ExportedTimer.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/Allocator/DefaultAllocator.h"

namespace Alice{
	int ExportedTimer::API_Reset(lua_State*L){
		ExportedTimer*ptr = TO_USERDATA(L, ExportedTimer, 1);
		ptr->Reset();
		return 0;
	}
	int ExportedTimer::API_GetTime(lua_State*L) {
		ExportedTimer*ptr = TO_USERDATA(L, ExportedTimer, 1);
		lua_pushinteger(L, ptr->GetTimeInMsi());
		return 1;
	}
	int ExportedTimer::Export(lua_State*L) {
		luaL_Reg apis[] = {
			EXPORT_API(Reset)
			EXPORT_API(GetTime)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Timer, apis);
		luaL_Reg api_c[] ={
			EXPORT_API_NEW
			EXPORT_API_END
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Timer, api_c);
		return 0;
	}
}