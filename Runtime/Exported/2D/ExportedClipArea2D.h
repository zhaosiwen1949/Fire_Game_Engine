#pragma once
#include "Runtime/2D/ClipArea2D.h"
namespace Alice{
	class ExportedClipArea2D : public ClipArea2D {
	public:
		DEFINE_LUA_API(SetSize);
		static int Export(lua_State*L);
	public:
		//c++ class¡¢lua 
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedClipArea2D, ClipArea2D)
	};
}
