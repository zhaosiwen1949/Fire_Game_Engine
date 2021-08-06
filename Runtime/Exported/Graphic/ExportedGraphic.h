#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Graphics/QRCodeImage.h"
namespace Alice{
	class ExportedGraphic{
	public:
		DEFINE_LUA_API(DecodeQRCodeImageData);
		DEFINE_LUA_API(Render);
		DEFINE_LUA_API(Baking);
		static int Export(lua_State*L);
	};
}
