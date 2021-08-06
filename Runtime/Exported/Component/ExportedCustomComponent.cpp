#include "ExportedCustomComponent.h"

namespace Alice
{
	int ExportedCustomComponent::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetUpdater)
			EXPORT_API(SetRenderer)
			EXPORT_API(ClassID)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, CustomComponent, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = { EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, CustomComponent, api_c);
		return 0;
	}

	int ExportedCustomComponent::API_SetUpdater(lua_State*L)
	{
		if (lua_gettop(L)!=2)
		{
			errorC("CustomComponent:SetUpdater nedd 2 arg");
			return 0;
		}
		if (!IsInvokeValid<CustomComponent>(L,1,"SetUpdater"))
		{
			errorC("CustomComponent:SetUpdater invoke not valid");
			return 0;
		}
		if (!lua_iscfunction(L,2))
		{
			errorC("CustomComponent:SetUpdater arg at 2 must be a c function");
			return 0;
		}
		ExportedCustomComponent*ptr = TO_USERDATA(L, ExportedCustomComponent, 1);
		ptr->mUpdater = luaL_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}

	int ExportedCustomComponent::API_SetRenderer(lua_State*L)
	{
		if (lua_gettop(L) != 2)
		{
			errorC("CustomComponent:SetRenderer nedd 2 arg");
			return 0;
		}
		if (!IsInvokeValid<CustomComponent>(L, 1, "SetRenderer"))
		{
			errorC("CustomComponent:SetRenderer invoke not valid");
			return 0;
		}
		if (!lua_iscfunction(L, 2))
		{
			errorC("CustomComponent:SetRenderer arg at 2 must be a c function");
			return 0;
		}
		ExportedCustomComponent*ptr = TO_USERDATA(L, ExportedCustomComponent, 1);
		ptr->mRenderer = luaL_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}
}