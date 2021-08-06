#pragma once
#include "Runtime/2D/Canvas.h"
namespace Alice {
	class ExportedCanvas : public Canvas {
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedCanvas, Canvas)
	};
}
