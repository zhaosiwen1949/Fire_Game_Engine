#include "Sprite2D.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
namespace Alice {
	Sprite2D::Sprite2D() {
		mMaterial= ExportedMaterial::Clone("builtin/UI");
		mMaterial->SetPrimitive(GL_TRIANGLE_STRIP);
		mVertexData.SetBufferSize(4);
		mAddtionalColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mVBO.Init(4);
		VertexDataFull *vertexes=mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mVertex.Set(-100.0f, -100.0f, 0.0f);
		vertexes[1].mVertex.Set(100.0f, -100.0f, 0.0f);
		vertexes[2].mVertex.Set(-100.0f, 100.0f, 0.0f);
		vertexes[3].mVertex.Set(100.0f, 100.0f, 0.0f);
		vertexes[0].mTexCoord0.Set(0.0f, 0.0f,0.0f);
		vertexes[1].mTexCoord0.Set(1.0f, 0.0f, 0.0f);
		vertexes[2].mTexCoord0.Set(0.0f, 1.0f, 0.0f);
		vertexes[3].mTexCoord0.Set(1.0f, 1.0f, 0.0f);
		mMask |= _00000000_00000000_00000000_00000001;
	}
	void Sprite2D::Update(float deltaTime) {
		if (mMask!=0) {
			mVBO.SubData(mVertexData.GetBuffer<VertexDataFull>(), sizeof(VertexDataFull) * 4);
			mMask = 0;
		}
	}
	void Sprite2D::Render(RenderQueue*rq) {
		if (rq->mCanvas==nullptr){
			return;
		}
		DrawCall *dc = new DrawCall;
		dc->mMaterial = mMaterial.mPtr;
		dc->mVBO = &mVBO;
		rq->AppendUIDrawCall(dc);
	}
	void Sprite2D::SetTexture(const char *image_path) {
		Texture2D*texture = Texture2D::LoadTexture2D(image_path);
		mMaterial->SetTextureProperty("U_Texture", texture);
		SetSize(texture->mWidth, texture->mHeight);
	}
	void Sprite2D::SetSize(float width, float height) {
		float half_width=width/2.0f, half_height=height/2.0f;
		VertexDataFull *vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mVertex.Set(-half_width, -half_height, 0.0f);
		vertexes[1].mVertex.Set(half_width, -half_height, 0.0f);
		vertexes[2].mVertex.Set(-half_width, half_height, 0.0f);
		vertexes[3].mVertex.Set(half_width, half_height, 0.0f);
		mMask |= _00000000_00000000_00000000_00000001;
	}
	void Sprite2D::UpdateVertexTangent(int nIndex, float x, float y, float z, float w /* = 1.0f */) {
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTangent.Set(x*mAddtionalColor.v[0], y*mAddtionalColor.v[1], z*mAddtionalColor.v[2], w*mAddtionalColor.v[3]);
		mMask |= _00000000_00000000_00000000_00000010;
	}

	void Sprite2D::UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w /* = 1.0f */) {
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTexCoord1.Set(x, y, z, w);
		mMask |= _00000000_00000000_00000000_00000010;
	}
	void Sprite2D::SetColor(float r, float g, float b, float a) {
		mColor.Set(ALICE_MIN(r, 1.0f), ALICE_MIN(g, 1.0f), ALICE_MIN(b, 1.0f), ALICE_MIN(a, 1.0f));
		for (int i = 0; i < 4; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
	}
}