#pragma once
#include "Runtime/Render/RenderOrder.h"
namespace Alice{
	class ExportedRenderOrder :public RenderOrder{
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedRenderOrder, RenderOrder)
	};
}