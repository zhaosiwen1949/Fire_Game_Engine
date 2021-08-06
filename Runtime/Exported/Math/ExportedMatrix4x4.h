#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Math/AliceMatrix4x4.h"
namespace Alice{
	class ExportedMatrix4x4 : public Matrix4x4 {
	public:
		DEFINE_LUA_API(Invert);
		DEFINE_LUA_API(Transpose);
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD_MANAGED(ExportedMatrix4x4, Matrix4x4);
	};
}
