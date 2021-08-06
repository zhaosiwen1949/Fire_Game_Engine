#pragma once
#include "Runtime/Render/VertexData.h"
#include "Runtime/2D/Component2D.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Render/GL20/Texture2D.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Math/Color.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/String/FixedString.h"
namespace Alice{
	class RenderQueue;
	class ImageSprite:public Component2D{
	public:
		FixedString mImage;
		VertexData  mVertexData;
		VBO mVBO;
		float mWidth;
		float mHeight;
		AliceUInt8 mDirtyMask;
		Color4f mColor;//
		Color4f mAddtionalColor;//
		std::string mSpriteName;
		SmartPtr<Material> mMaterial;
		ImageSprite();
		virtual ~ImageSprite();
		OPT(bool) SetTexture(const char*uniformName, Texture2D* texture);
		OPT(bool) SetTexture(const char*uniformName,const char* texture);
		OPT(bool) SetMaterial(Material*material);
		OPT(bool) SetMaterial(const char*matname);
		OPT(bool) SetSpriteByName(const char*spriteName);
		void SetSprite(Serializer::Sprite*sprite);
		void SetSprite(Serializer::ImageTile*imageTile);
		void SetColor(Color4B&color);
		void SetColor(Color4f&color);
		void SetColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a);
		void SetColor(float r, float g, float b, float a);
		void SetAdditionalColor(float r, float g, float b, float a);
		void UpdateVertexData();
		void ResetToStandardTexcoord();
		void UpdateVertexNormal(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTangent(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w = 1.0f);
		void SetTexcoordData(Vector4f*texcoords);
		void UpdateBoundingBox();
		void SetWidth(float value);
		void SetHeight(float value);
		void SetSize(float w, float h);
		void SetAlpha(AliceUInt8 alpha);
		virtual Component* Clone();
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
	public:
		static AliceUInt16 mIndex[6];
	public:
		DECLEAR_ALICE_CLASS(ImageSprite)
	};
}