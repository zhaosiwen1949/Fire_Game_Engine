#pragma once
#include "Runtime/3D/Mesh.h"

namespace Alice
{
	class ExportedMesh:public Mesh
	{
	public:
		//count
		DEFINE_LUA_API(SetVertexCount);
		//index,x,y,z
		DEFINE_LUA_API(UpdatePosition);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTexcoord);
		//index,x,y,z
		DEFINE_LUA_API(UpdateNormal);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTangent);
		//index,x,y,z,w
		DEFINE_LUA_API(UpdateTexcoord1);
		//submesh count
		DEFINE_LUA_API(SetSubMeshCount);
		//submesh index count
		DEFINE_LUA_API(SetSubMeshIndexCount);
		//index,value
		DEFINE_LUA_API(UpdateSubMeshIndex);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedMesh, Mesh)
	};
}
