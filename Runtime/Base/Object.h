#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/LuaEngine/LuaEngine.h"
#include "Runtime/String/FixedString.h"
#define DEFINE_TYPE_ID(T,ID) const int TYPEID_##T=ID;
#define ALICE_TYPE_ID(T) TYPEID_##T
#include "ClassID.h"
#define DECLEAR_ALICE_CLASS(T) \
virtual int GetClassID(){return ALICE_TYPE_ID(T);}\
static int API_ClassID(lua_State*L){lua_pushinteger(L,ALICE_TYPE_ID(T));return 1;} \
static const int ClassID=ALICE_TYPE_ID(T); \
static const char* GetTypeName(){return #T;} \
static bool Is##T(lua_State*L,int index) { \
	Alice::LuaEngine::Store(); \
	if(Object::IsValid(L,index)) \
	{ \
		lua_getfield(L, index, "ClassID"); \
		if(lua_isfunction(L,-1))\
		{\
			lua_pcall(L, 0, 1, 0); \
			int classID=lua_tointeger(L,-1);\
			if(classID==ClassID)\
			{\
				Alice::LuaEngine::Restore();\
				return true;\
			}\
			else\
			{\
				Alice::LuaEngine::Restore();\
				return false;\
			}\
		}\
		else\
		{\
			Alice::LuaEngine::Restore();\
			return false;\
		}\
	}\
	else\
	{\
		Alice::LuaEngine::Restore();\
		return false;\
	}\
	Alice::LuaEngine::Restore();\
	return false;\
}
#define CreateLuaDataViaExistObject(ptr,T,L) \
do\
{\
T**pp = (T**)lua_newuserdata(L, sizeof(T*));\
*pp=ptr;\
SET_LUA_TYPE(L, T);\
ptr->mLuaData = luaL_ref(L, LUA_REGISTRYINDEX);\
}while(0)
namespace Alice {
	class Object {
	public:
		Object(int autorelease = 1, int name_len = 64);
		virtual ~Object();
	public:
		AliceSInt32 mReferenceCount;
		int mLuaData;
		int mObjectMask;
		FixedString mName;
	public:
		void retain();
		void release();
	public:
		void GetUserData(lua_State*L);
		void PushUserData(lua_State*L) {
			if (mLuaData != LUA_REFNIL) {
				lua_getref(L, mLuaData);
			}
			else {
				lua_pushnil(L);
			}
		}
		static int DefaultGC(lua_State*L);
		static bool IsValid(lua_State*L, int index);
		DECLEAR_ALICE_CLASS(Object)
	};
	template<typename T>
	T*CreateExportedAliceType() {
		lua_State*L = LuaEngine::Store();
		T::New_ExportedAliceType(L);
		T*pObject = (*(T**)lua_touserdata(L, -1));
		LuaEngine::Restore();
		return pObject;
	}
}
template<typename T>
bool IsType(lua_State*L, int i) {
	if (lua_isnil(L, i)) {
		return false;
	}
	bool ret = false;
	lua_getfield(L, i, "ClassID");
	if (lua_isfunction(L, -1)) {
		lua_pcall(L, 0, 1, 0);
		if (lua_isnumber(L, -1)) {
			if (lua_tonumber(L, -1) == T::ClassID) {
				ret = true;
			}
		}
	}
	lua_pop(L, 1);
	return ret;
}
template<typename T>
bool IsInvokeValid(lua_State*L, int i, const char*method) {
	bool ret = false;
	if (IsType<T>(L, i)) {
		lua_getfield(L, i, method);
		if (lua_isfunction(L, -1)) {
			ret = true;
		}
		lua_pop(L, 1);
	}
	return ret;
}
#define CreateLuaDataViaExistObject(ptr,T,L) \
do\
{\
T**pp = (T**)lua_newuserdata(L, sizeof(T*));\
*pp=ptr;\
SET_LUA_TYPE(L, T);\
ptr->mLuaData = luaL_ref(L, LUA_REGISTRYINDEX);\
}while(0)