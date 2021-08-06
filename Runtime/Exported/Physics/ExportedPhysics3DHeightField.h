#pragma once
#include "Runtime/Physics/Physics3DHeightField.h"
namespace Alice{
	class ExportedPhysics3DHeightField :public Physics3DHeightField {
	public:
		static ExportedPhysics3DHeightField*New(void*mesh);
		static int Export(lua_State*L);
		DEFINE_LUA_API(SetSize);
		DEFINE_LUA_API(GetSize);
		DEFINE_LUA_API(SetVelocity);
		DEFINE_LUA_API(GetVelocity);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedPhysics3DHeightField, Physics3DHeightField)
	};
}
