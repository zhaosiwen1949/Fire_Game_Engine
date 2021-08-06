#pragma once
#include "Runtime/Physics/Physics3DTriangleMesh.h"
namespace Alice{
	class ExportedPhysics3DTriangleMesh :public Physics3DTriangleMesh {
	public:
		static ExportedPhysics3DTriangleMesh*New(void*mesh);
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
		DEFINE_LUA_API(SetVelocity);
		DEFINE_LUA_API(GetVelocity);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DTriangleMesh, Physics3DTriangleMesh)
	};
}
