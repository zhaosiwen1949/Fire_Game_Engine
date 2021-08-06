#pragma once
#include "Runtime/Render/GL20/TextureCube.h"

//2016 08/24 2.8.7 pass 

namespace Alice
{
	class ExportedTextureCube :public TextureCube
	{
	public:
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedTextureCube, TextureCube)
	};
}