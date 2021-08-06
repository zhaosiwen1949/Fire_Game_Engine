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
	class Sprite2D : public Component2D{
	public:
		VertexData  mVertexData;
		VBO mVBO;
		Color4f mColor;//
		Color4f mAddtionalColor;//
		SmartPtr<Material> mMaterial;
		AliceUInt32 mMask;
	public:
		Sprite2D();
		void Update(float deltaTime);
		void Render(RenderQueue*rq);
		void SetTexture(const char *image_path);
		void SetSize(float width, float height);
		void SetColor(float r, float g, float b, float a);
		void UpdateVertexTangent(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w = 1.0f);
	public:
		DECLEAR_ALICE_CLASS(Sprite2D)
	};
}