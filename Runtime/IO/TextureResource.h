#pragma once
#include "AliceResource.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"
namespace Alice{
	enum TextureResourceType{
		kTextureResourceTypeTexture,
		kTextureResourceTypeTextureInfo,
		kTextureResourceTypeCount
	};
	class TextureResource :public Resource{
	public:
		TextureResourceType mTextureResourceType;
	public:
		static TextureResource* LoadTexture(Serializer::RuntimeAsset*asset);
		static TextureResource* CreateTextureResourceFromRawData(const char*path,Data&data);
		DECLEAR_ALICE_CLASS(TextureResource)
	}; 
}