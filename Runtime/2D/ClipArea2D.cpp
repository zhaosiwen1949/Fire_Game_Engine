#include "ClipArea2D.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
namespace Alice {
	ClipArea2D::ClipArea2D() {
		mMaterial= ExportedMaterial::Clone("builtin/StencilUI");
		mMaterial->SetPrimitive(GL_TRIANGLE_STRIP);
		mVertexData.SetBufferSize(4);
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
	void ClipArea2D::Update(float deltaTime) {
		if (mMask!=0) {
			mVBO.SubData(mVertexData.GetBuffer<VertexDataFull>(), sizeof(VertexDataFull) * 4);
			mMask = 0;
		}
	}
	void ClipArea2D::Render(RenderQueue*rq) {
		if (rq->mCanvas==nullptr){
			return;
		}
		DrawCall *dc = new DrawCall;
		dc->mMaterial = mMaterial.mPtr;
		dc->mVBO = &mVBO;
		rq->AppendUIDrawCall(dc);
	}
	void ClipArea2D::SetSize(float width, float height) {
		float half_width=width/2.0f, half_height=height/2.0f;
		VertexDataFull *vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mVertex.Set(-half_width, -half_height, 0.0f);
		vertexes[1].mVertex.Set(half_width, -half_height, 0.0f);
		vertexes[2].mVertex.Set(-half_width, half_height, 0.0f);
		vertexes[3].mVertex.Set(half_width, half_height, 0.0f);
		mMask |= _00000000_00000000_00000000_00000001;
	}
}