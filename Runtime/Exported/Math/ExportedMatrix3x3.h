#pragma once
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Math/Matrix3x3.h"
namespace Alice{
	class ExportedMatrix3x3 : public Matrix3x3 {
	public:
		DEFINE_LUA_API(Invert);
		DEFINE_LUA_API(Transpose);
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD_MANAGED(ExportedMatrix3x3, Matrix3x3);
	};
}
