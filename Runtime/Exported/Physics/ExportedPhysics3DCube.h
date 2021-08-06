#pragma once
#include "Runtime/Physics/Physics3DCube.h"
namespace Alice{
	class ExportedPhysics3DCube :public Physics3DCube {
	public:
		static ExportedPhysics3DCube*New();
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
		DEFINE_LUA_API(SetVelocity);
		DEFINE_LUA_API(GetVelocity);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DCube, Physics3DCube)
	};
}
