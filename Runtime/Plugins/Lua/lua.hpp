#pragma once
extern "C"{
#include "External/Lua/LuaVMLib/lua.h"
#include "External/Lua/LuaVMLib/lualib.h"
#include "External/Lua/LuaVMLib/lauxlib.h"
}
#define MODULE_NAME(X) #X
#define METHOD_NAME(N) #N
#define EXPORT_NAME(N) "Alice."#N
#define METATABLENAME(T) "MT_"#T

#if ALICE_WIN_PLAYER || ALICE_WIN_EDITOR
#define ALICE_MODULE(X) Alice::##X
#else
#define ALICE_MODULE(X) Alice::X
#endif

#define LUA_TYPE(l,i) lua_typename(l,lua_type(l,i))
#define API_NAME(name) API_##name
#define DEFINE_LUA_API(name) static int API_##name(lua_State*L)
#define EXPORT_API(name) {#name,API_##name},
#define EXPORT_API_NEW {"New", New_ExportedAliceType},
#define EXPORT_API_END {NULL,NULL}

#define SET_PARENT(L,parent) do \
{\
	luaL_getmetatable(L, METATABLENAME(parent)); \
	lua_setmetatable(L, -2); \
} while (0)

#define SET_LUA_TYPE(L,t) SET_PARENT(L,t)
#define UNSET_LUA_TYPE(L) do { \
	lua_pushnil(L); \
	lua_setmetatable(L, -2); \
}while(0)

#define NEW_LUA_TYPE_WITH_API(L,t,mf) do \
{ \
	luaL_newmetatable(L, METATABLENAME(t));\
	lua_pushstring(L,MODULE_NAME(t));\
	lua_setfield(L,-2,"Name");\
	lua_pushvalue(L, -1);\
	lua_setfield(L, -2, "__index");\
	luaL_openlib(L, NULL, mf, 0);\
	lua_pushcfunction(L,t::DefaultGC);\
	lua_setfield(L,-2,"__gc");\
}while(0)

#define APPEND_LUA_PACKAGE_API(L,P,F,CF) do { \
lua_getglobal(L,P);\
lua_pushcfunction(L,CF);\
lua_setfield(L,-2,F);\
lua_pop(L,1);\
} while (0)

#define EXPORT_ALICE_PACKAGE_TO_SCRIPT(L,t,cf) do \
{ \
	luaL_openlib(L, EXPORT_NAME(t), cf, 0);\
}while(0)

#define DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(cpp_type,luatype) static int New_ExportedAliceType(lua_State*L) \
{\
	cpp_type**pp=(cpp_type**)lua_newuserdata(L,sizeof(cpp_type*)); \
	*pp=new (kMemDefaultId)cpp_type; \
	cpp_type*pObject = *pp; \
	SET_LUA_TYPE(L, luatype);\
	pObject->GetUserData(L); \
	return 1; \
}

#define DEF_ALICE_EXPORTED_TYPE_NEW_METHOD_MANAGED(cpp_type,luatype) static int New_ExportedAliceType(lua_State*L) \
{\
	cpp_type**pp=(cpp_type**)lua_newuserdata(L,sizeof(cpp_type*)); \
	*pp=new (kMemDefaultId)cpp_type; \
	(*pp)->retain(); \
	cpp_type*pObject = *pp; \
	SET_LUA_TYPE(L, luatype);\
	return 1; \
}

#define INIT_LUAMODULE(L,X) do \
{	\
	lua_pushcfunction(L, ALICE_MODULE(X)::Export); lua_pushstring(L,MODULE_NAME(X));\
	if(lua_pcall(L,1,0,0)!=0){printf("%s\n",lua_tostring(L,-1));} \
}while(0)

#define DEFINE_C_MODULE_ENTRY(X) extern "C" int luaopen_##X(lua_State*)
#define INIT_CLUAMODULE(L,X) do \
{\
	lua_pushcfunction(L, luaopen_##X); lua_pushstring(L,"luaopen_"#X);\
	if(lua_pcall(L,1,0,0)!=0){printf("%s\n",lua_tostring(L,-1));} \
}while(0)

#define TO_USERDATA(L,t,idx) (*(t**)lua_touserdata(L,idx))

#define SAFE_INVOKE_LUA_METHOD_ARG0(m)do{\
    if(m!=LUA_REFNIL){lua_State *L = Alice::LuaEngine::Store();lua_getref(L, m);\
	if(lua_isfunction(L,-1)){if (lua_pcall(L, 0, 0, -2) != 0){ Error("%s",lua_tostring(L, -1));}\
}Alice::LuaEngine::Restore();}}while(0)
#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(t,m) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){lua_getref(L, t);\
			if (lua_pcall(L, 1, 0, -4) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)
#define SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG0(t,m) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){\
			if (lua_pcall(L, 0, 0, -2) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)
#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(t,m) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getref(L, m);\
		if(lua_isfunction(L,-1)){lua_getref(L, t);\
			if (lua_pcall(L, 1, 0, -3) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)

#define SAFE_INVOKE_LUA_METHOD_ARG1F(m,arg1)do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, m);\
	if(lua_isfunction(L,-1)){lua_pushnumber(L,arg1); if (lua_pcall(L, 1, 0, -3) != 0){ Error("%s",lua_tostring(L, -1));}\
}Alice::LuaEngine::Restore();}while(0)
#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(t,m,arg1) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){lua_getref(L, t);lua_pushnumber(L,arg1); \
			if (lua_pcall(L, 2, 0, -4) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)
#define SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG1F(t,m,arg1) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){lua_pushnumber(L,arg1); \
			if (lua_pcall(L, 1, 0, -3) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)

#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(t,m,arg1) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, m);\
		if(lua_isfunction(L,-1)){lua_getref(L, t);lua_pushnumber(L,arg1); \
			if (lua_pcall(L, 2, 0, -4) != 0){ Error("%s",lua_tostring(L, -1));}\
}Alice::LuaEngine::Restore();}while(0)
#define FAST_SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG1F(t,m,arg1) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getref(L, m);\
		if(lua_isfunction(L,-1)){lua_pushnumber(L,arg1); \
			if (lua_pcall(L, 1, 0, -3) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)
#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG2F(t,m,arg1,arg2) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){ lua_getref(L,t);\
			lua_pushnumber(L,arg1);lua_pushnumber(L,arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)
#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_2I(t,m,arg1,arg2) do{\
	lua_State *L = Alice::LuaEngine::Store();lua_getref(L, t);\
	if(lua_istable(L,-1)){lua_getref(L, m);\
		if(lua_isfunction(L,-1)){lua_getref(L, t);lua_pushinteger(L,arg1); lua_pushinteger(L,arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0){ Error("%s",lua_tostring(L, -1));}\
}}Alice::LuaEngine::Restore();}while(0)

#define SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(t,m,a1,a2,a3) do{\
	if(m!=LUA_REFNIL){\
		lua_State *L = Alice::LuaEngine::Store();\
		lua_getref(L, m);\
		lua_getref(L, t);\
		lua_pushnumber(L, a1);\
		lua_pushnumber(L, a2);\
		lua_pushinteger(L, AliceSInt32(a3));\
		if (lua_pcall(L, 4, 0, -6) != 0){\
			Error("%s",lua_tostring(L, -1));\
		}\
		Alice::LuaEngine::Restore();\
	}\
}while(0)
#define SAFE_INVOKE_LUA_CLASS_METHOD_1I(t,m,a1) do{\
	if(m!=LUA_REFNIL){\
		lua_State *L = Alice::LuaEngine::Store();\
		lua_getref(L, m);\
		lua_getref(L, t);\
		lua_pushinteger(L, a1);\
		if (lua_pcall(L, 2, 0, -4) != 0){\
			Error("%s",lua_tostring(L, -1));\
		}\
		Alice::LuaEngine::Restore();\
	}\
}while(0)
#define SAFE_INVOKE_LUA_CLASS_METHOD_2I(t,m,arg1,arg2) do{\
	lua_State *L = Alice::LuaEngine::Store();\
	lua_getref(L, t);\
	if(lua_istable(L,-1)){ \
		lua_getfield(L, -1, METHOD_NAME(m));\
		if(lua_isfunction(L,-1)){ lua_getref(L,t);\
			lua_pushinteger(L,arg1);lua_pushinteger(L,arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0){ \
				Error("%s",lua_tostring(L, -1)); \
			}\
		}\
	}\
Alice::LuaEngine::Restore();}while(0)

#define CheckArgCount(n,l,c) if(lua_gettop(l)<c){\
char szBuffer[256]={0};sprintf(szBuffer,"%s need more args,now arg count is %d",#n,lua_gettop(l));\
lua_pushboolean(l,false);lua_pushstring(l,szBuffer);return 2;}

#define UNREF(L,obj) \
do{if(LUA_REFNIL!=obj) {lua_unref(L,obj);obj=LUA_REFNIL;}}while(0)
#define REF_FIELD(L,idx,obj,name) \
do{ \
	lua_getfield(L, idx, name);\
	if (!lua_isnil(L, -1)) { \
		obj=luaL_ref(L,LUA_REGISTRYINDEX);\
	}else{\
		lua_pop(L,1);\
	}\
}while(0)
#define REF_TOP(L,obj) \
do{if(LUA_REFNIL!=obj) {lua_unref(L,obj);obj=LUA_REFNIL;}}while(0)
