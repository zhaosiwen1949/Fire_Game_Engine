#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/3D/Terrain.h"
namespace Alice{
	class ExportedTerrain :public Terrain {
	public:
		DEFINE_LUA_API(SetHeightMap);
		DEFINE_LUA_API(SetMaterial);
		DEFINE_LUA_API(SetHeightScale);
		DEFINE_LUA_API(SetXZScale);
		DEFINE_LUA_API(GetInfo);
		DEFINE_LUA_API(GetHeight);
		DEFINE_LUA_API(GetNormal);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedTerrain, Terrain)
	};
}
