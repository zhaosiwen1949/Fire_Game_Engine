#pragma once
#include "Runtime/Geometry/Ray.h"
#include "Runtime/Allocator/DefaultAllocator.h"
//��������ͼ�ε��ű��㣬����������߼�
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
