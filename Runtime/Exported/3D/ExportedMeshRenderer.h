#pragma once
#include "Runtime/3D/MeshRenderer.h"
namespace Alice{
	class ExportedMeshRenderer :public MeshRenderer {
	public:
		DEFINE_LUA_API(SetMaterialCount);
		DEFINE_LUA_API(SetMaterial);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedMeshRenderer, MeshRenderer)
	};
}
