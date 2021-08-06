#pragma once
#include "Runtime/Physics/Physics3DConvexMesh.h"
namespace Alice{
	class ExportedPhysics3DConvexMesh :public Physics3DConvexMesh {
	public:
		static ExportedPhysics3DConvexMesh*New();
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DConvexMesh, Physics3DConvexMesh)
	};
}
