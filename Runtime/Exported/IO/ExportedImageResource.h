#pragma once
#include "Runtime/Render/AliceGL.h"
#include "Runtime/IO/ImageResource.h"
//raw image data in ram not video card for other purpose
namespace Alice{
	class ExportedImageResource : public ImageResource {
	public:
		//get x,y pixel value(r,g,b,a)
		DEFINE_LUA_API(GetColor);
		DEFINE_LUA_API(GetSize);
		DEFINE_LUA_API(GetPath);
		DEFINE_LUA_API(Load);
		DEFINE_LUA_API(Unload);
		DEFINE_LUA_API(SetFileData);
		DEFINE_LUA_API(SetImageData);
		DEFINE_LUA_API(GetImageData);
		DEFINE_LUA_API(SetImageInfo);
		DEFINE_LUA_API(GetImageInfo);
		DEFINE_LUA_API(Scale);
		DEFINE_LUA_API(Save);
		static int Export(lua_State*L);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedImageResource, ImageResource)
	};
}
