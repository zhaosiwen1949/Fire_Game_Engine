#pragma once
#include "Runtime/Physics/Physics3DComponent.h"

namespace Alice{
	class ExportedPhysics3DComponent :public Physics3DComponent {
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetLinearVelocity);
		DEFINE_LUA_API(GetLinearVelocity);
		DEFINE_LUA_API(EnableGravity);
		DEFINE_LUA_API(LockVelocity);
		DEFINE_LUA_API(LockAngular);
		DEFINE_LUA_API(SetOffset);
		DEFINE_LUA_API(Test);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DComponent, Physics3DComponent)
	};
}
