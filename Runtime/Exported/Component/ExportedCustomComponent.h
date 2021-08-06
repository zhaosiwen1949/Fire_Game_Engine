#pragma once
#include "Runtime/3D/CustomComponent.h"

namespace Alice
{
	class ExportedCustomComponent :public CustomComponent
	{
	public:
		DEFINE_LUA_API(SetUpdater);
		DEFINE_LUA_API(SetRenderer);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedCustomComponent,CustomComponent)
	};
}
