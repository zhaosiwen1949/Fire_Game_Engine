#pragma once
#include "Runtime/Physics/Physics3DCapsule.h"
namespace Alice{
	class ExportedPhysics3DCapsule :public Physics3DCapsule {
	public:
		static ExportedPhysics3DCapsule*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetInfo);
		DEFINE_LUA_API(GetInfo);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DCapsule, Physics3DCapsule)
	};
}
