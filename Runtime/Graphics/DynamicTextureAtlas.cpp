#include "DynamicTextureAtlas.h"
#include "Runtime/Scene/RendererManager.h"

namespace Alice
{
	std::vector<DynamicTextureAtlas*> DynamicTextureAtlas::mCachedAtlas;
	DynamicTextureAtlas::DynamicTextureAtlas(AliceUInt16 width /* = 4096 */, AliceUInt16 height /* = 4096 */):mMaterial(nullptr)
	{
		mTexture = new (kMemTextureId)Texture2D;
		mTexture->InitTexture2D(width, height);
		mTexture->mName = "DynamicTextureAtlas";
		mRootNode = new (kMemTextureId)TextureRectangleNode(true);
		mRootNode->mRect.Set(0, 0, width, height);
		Material*mat = new (kMemMaterialId)Material;
		//*mat = *GetRendererManager().mpDynamicFontMaterial;
		mMaterial = mat;
		mMaterial->SetTextureProperty("U_MainTexture", mTexture);
		mTexture->retain();
		mCachedAtlas.push_back(this);
	}

	TextureRectangleNode*DynamicTextureAtlas::AddTextureTile(AliceUInt64 ID, int width, int height, AliceUInt8 *imageData)
	{
		if (mTextureTiles.find(ID) == mTextureTiles.end())
		{
			TextureRectangleNode* newNode = mRootNode->AddTexture(width, height);
			if (newNode != nullptr)
			{
				mTextureTiles.insert(std::pair<AliceUInt64, TextureRectangleNode*>(ID, newNode));
				mTexture->UploadSubTexture(newNode->mRect.mLeft, newNode->mRect.mBottom, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				newNode->mTexCoord.x = (float)newNode->mRect.mLeft / (float)mRootNode->mRect.mWidth;//left
				newNode->mTexCoord.y = (float)newNode->mRect.mBottom / (float)mRootNode->mRect.mHeight;//bottom
				newNode->mTexCoord.z = (float)(newNode->mRect.mLeft + newNode->mRect.mWidth) / (float)mRootNode->mRect.mWidth;//right
				newNode->mTexCoord.w = (float)(newNode->mRect.mBottom + newNode->mRect.mHeight) / (float)mRootNode->mRect.mHeight;//top
				return newNode;
			}
			else
			{
				return nullptr;
			}
		}
		return mTextureTiles[ID];
	}

	DynamicTextureAtlas*DynamicTextureAtlas::GetDefaultDynamicTextureAtlas()
	{
		if (mCachedAtlas.empty())
		{
			return new DynamicTextureAtlas;
		}
		else
		{
			return mCachedAtlas.back();
		}
	}
}