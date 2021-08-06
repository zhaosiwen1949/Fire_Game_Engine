#include "DrawCall.h"
#include "Light.h"
DrawCall::DrawCall() {
	mVBO = nullptr;
	mIBO = nullptr;
	mMaterial = nullptr;
}
void DrawCall::Draw(Camera* camera) {
	switch (mCatalog) {
	case kRenderCatalogForward:
		ForwardRendering(camera);
		break;
	case kRenderCatalogDefferedGPass:
		DefferedGPass(camera);
		break;
	case kRenderCatalogDefferedLightPass:
		DefferedLightPass(camera);
		break;
	}
}
void DrawCall::ForwardRendering(Camera* camera) {
	mVBO->Bind();
	// set main light
	// material->base->bind(camera)
	// vbo->draw
	static float main_light_pos[] = { -1.0f,1.0f,0.0f,0.0f };
	mMaterial->mBaseRenderPass->SetVec4("U_LightPosition", main_light_pos);
	mMaterial->mBaseRenderPass->SetVec4("U_LightColor", Light::mMainLight->mDiffuse);
	mMaterial->mBaseRenderPass->Bind(camera);//program -> bind 
	//lightmaps -> bind
	//ambient,emissive
	//shadow map
	//Light::mMainLight->Bind 
	if (mIBO == nullptr) {
		glDrawArrays(GL_TRIANGLES, 0, mVBO->mVertexCount);
	}
	//for light in lights
	//   set light state
	//   material->add->bind(camera)
	//   vbo->draw
	for (auto iter = Light::mLights.begin(); iter != Light::mLights.end(); ++iter) {//1000
		static float add_light_pos[] = { 1.0f,1.0f,0.0f,0.0f };
		mMaterial->mAdditiveRenderPass->SetVec4("U_LightPosition", add_light_pos);
		mMaterial->mAdditiveRenderPass->SetVec4("U_LightColor", (*iter)->mDiffuse);
		mMaterial->mAdditiveRenderPass->Bind(camera);//program -> bind
		//light -> binid
		if (mIBO == nullptr) {
			glDrawArrays(GL_TRIANGLES, 0, mVBO->mVertexCount);
		}
	}//300
	//->deffered
	mVBO->Unbind();
	if (mNext != nullptr) {
		Next<DrawCall>()->Draw(camera);
	}
}
void DrawCall::DefferedGPass(Camera* camera) {
	mVBO->Bind();
	mMaterial->mBaseRenderPass->Bind(camera);//program -> bind 
	if (mIBO == nullptr) {
		glDrawArrays(GL_TRIANGLES, 0, mVBO->mVertexCount);
	}
	mVBO->Unbind();
	if (mNext != nullptr) {
		Next<DrawCall>()->Draw(camera);
	}
}
void DrawCall::DefferedLightPass(Camera* camera) {

}