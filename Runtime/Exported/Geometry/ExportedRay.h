#pragma once
#include "Runtime/Geometry/Ray.h"
#include "Runtime/Allocator/DefaultAllocator.h"
//导出几何图形到脚本层，方便后面做逻辑
namespace Alice
{
	class ExportedRay :public Ray {
	public:
		DEFINE_LUA_API(GetOrigin);
		DEFINE_LUA_API(GetDirection);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedRay, Ray)
	};
}
