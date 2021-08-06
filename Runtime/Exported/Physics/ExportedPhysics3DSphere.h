#pragma once
#include "Runtime/Physics/Physics3DSphere.h"
namespace Alice{
	class ExportedPhysics3DSphere :public Physics3DSphere {
	public:
		DEFINE_LUA_API(SetOffset);
		DEFINE_LUA_API(SetRadius);
		DEFINE_LUA_API(GetPosition);
		DEFINE_LUA_API(GetRadius);
	public:
		static ExportedPhysics3DSphere*New();
		static int Export(lua_State*L);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DSphere, Physics3DSphere)
	};
}
