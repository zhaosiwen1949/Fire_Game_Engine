#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/GL20/Texture2D.h"
#include "Runtime/Render/Material.h"
#include "TextureRectangleNode.h"
#include "Runtime/Utils/SmartPtr.h"

namespace Alice{
	class DynamicTextureAtlas :public Object {
	public:
		DynamicTextureAtlas(AliceUInt16 width = 4096, AliceUInt16 height = 4096);
		std::map<AliceUInt64, TextureRectangleNode*> mTextureTiles;
		TextureRectangleNode*mRootNode;
		Texture2D* mTexture;
		SmartPtr<Material> mMaterial;
		TextureRectangleNode* AddTextureTile(AliceUInt64 ID, int width, int height, AliceUInt8 *imageData);
		static DynamicTextureAtlas* GetDefaultDynamicTextureAtlas();
		static std::vector<DynamicTextureAtlas*> mCachedAtlas;
	};
}