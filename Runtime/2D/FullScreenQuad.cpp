#include "FullScreenQuad.h"
#include "Runtime/Render/RenderPass.h"
namespace Alice{
	FullScreenQuad::FullScreenQuad() {

	}
	FullScreenQuad::~FullScreenQuad() {

	}
	void FullScreenQuad::Init() {
		mVBO.Init(4);
		VertexDataFull vertexes[4];
		float scale = 1.0f;
		vertexes[0].mVertex.Set(-1.0f*scale, -1.0f*scale, -1.0f);
		vertexes[0].mTexCoord0.Set(0.0f, 0.0f, 0.0f);
		vertexes[1].mVertex.Set(1.0f*scale, -1.0f*scale, -1.0f);
		vertexes[1].mTexCoord0.Set(1.0f, 0.0f, 0.0f);
		vertexes[2].mVertex.Set(-1.0f*scale, 1.0f*scale, -1.0f);
		vertexes[2].mTexCoord0.Set(0.0f, 1.0f, 0.0f);
		vertexes[3].mVertex.Set(1.0f*scale, 1.0f*scale, -1.0f);
		vertexes[3].mTexCoord0.Set(1.0f, 1.0f, 0.0f);
		mVBO.SubData(vertexes, sizeof(VertexDataFull) * 4);
	}
	void FullScreenQuad::Render(Material*material) {
		RenderPass*rp = material->mRenderPasses.front();
		mVBO.Active();
		rp->ActiveRenderState();
		rp->UpdateUniforms(nullptr);
		rp->SetupStencilBuffer();
		mVBO.Draw(GL_TRIANGLE_STRIP);
		rp->RestoreStencilBuffer();
		mVBO.Restore();
	}
}