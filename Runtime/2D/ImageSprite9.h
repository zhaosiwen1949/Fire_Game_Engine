#pragma once
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/2D/Component2D.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Render/GL20/EBO.h"
#include "Runtime/Render/GL20/Texture2D.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"

namespace Alice{
	//9宫格的图片显示方式，之所以不让九宫格继承ImageSprite，是因为这样独立写九宫格的图像会效率高一点
	class ImageSprite9:public Component2D{
	public:
		DECLEAR_ALICE_CLASS(ImageSprite9)
	public:
		FixedString mImage;
		VertexData  mVertexData;
		VBO mVBO;
		EBO	mEBO;
		SmartPtr<Material> mMaterial;
		//九宫格中间的方形格子
		float mLeftMargin;
		float mRightMargin;
		float mTopMargin;
		float mBottomMargin;
		float mWidth;
		float mHeight;

		float mOriginalWidth, mOriginalHeight;
		AliceUInt8 mDirtyMask;
		bool mbRotateTexcoord;
		Vector2f mSpriteTexcoordU;
		Vector2f mSpriteTexcoordV;
		Color4f mColor;
		Color4f mAddtionalColor;
		std::string mSpriteName;
		//单独为九宫格做一套数据存储方式，效率会更高一点
		ImageSprite9();
		virtual ~ImageSprite9();
		OPT(void) SetTexture(const char*uniformName,const char*path);
		OPT(void) SetTexture(const char*uniformName, Texture2D*texture);
		OPT(bool) SetMaterial(Material*material);
		OPT(bool) SetMaterial(const char*matname);
		OPT(bool) SetSpriteByName(const char*spriteName);
		void SetColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a);
		void SetColor(float r, float g, float b, float a);
		void SetAdditionalColor(float r, float g, float b, float a);
		void SetWidth(float value);
		void SetHeight(float value);
		void SetSize(float w, float h);
		void SetAlpha(AliceUInt8 alpha);
		void SetOriginalSize(float width, float height);
		void UpdateByTexture(Texture2D*texture);
		void SetAtlas(std::string atlasName);
		void SetSprite(Serializer::Sprite*sprite);
		void SetSplit(float top, float right,float bottom,float left);
		void BlendFunc(GLenum src, GLenum dst);
		void UpdateVertexNormal(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTangent(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexData();
		void UpdateTexcoordData();
		void UpdateBoundingBox();
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
	};
}