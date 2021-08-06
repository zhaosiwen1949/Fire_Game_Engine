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
	class ClipArea2D : public Component2D{
	public:
		VertexData  mVertexData;
		VBO mVBO;
		SmartPtr<Material> mMaterial;
		AliceUInt32 mMask;
	public:
		ClipArea2D();
		void Update(float deltaTime);
		void Render(RenderQueue*rq);
		void SetSize(float width, float height);
	public:
		DECLEAR_ALICE_CLASS(ClipArea2D)
	};
}