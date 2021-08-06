#include "DrawCall.h"
#include "Runtime/Render/light/Light.h"
#include "Runtime/Scene/GameObject.h"
#include "Camera.h"
#include "Material.h"
#include "VertexData.h"
#include "Runtime/Render/RenderState.h"
#include "RenderPass.h"

namespace Alice{
	DrawCall::DrawCall() :mVBO(nullptr), mEBO(nullptr), mMaterial(nullptr),mCamera(nullptr),mCustomComponent(nullptr),mGameObject(nullptr){
		mDistanceToCamera = 0.0f;
	}
	DrawCall::~DrawCall(){
		Clear();
	}
	void DrawCall::Clear(){
		DrawCall*next = Next<DrawCall>();
		if (next !=nullptr){
			delete next;
			mNext = nullptr;
		}
		if (mMaterial!=nullptr){
			mEBO = nullptr;
			mVBO = nullptr;
			mMaterial = nullptr;
			mCamera = nullptr;
		}
	}
	void DrawCall::SetUpDepthMap(RenderPass*pass) {
		GameObject*currentGO = (GameObject*)mGameObject;
		if (mMaterial->mbReceiveShadow&&mCamera->mbRenderDepth) {//camera rendered depth map should pass to the material who need it
			if (Light::mMainLight != nullptr&&currentGO != nullptr) {
				if ((currentGO->mLayer&Light::mMainLight->mCullingMask) != 0) {
					pass->SetTextureProperty(UniformDefines::Sampler_DepthMap_NameInShader, mCamera->mDepthFBO->mColorBuffer.mPtr);
					pass->SetMatrix4Property(UniformDefines::Matrix_MainLightProjection_NameInShader, Light::mMainLight->mProjection.mData);
					pass->SetMatrix4Property(UniformDefines::Matrix_MainLightView_NameInShader, Light::mMainLight->mView.mData);
					pass->SetVec4Property(UniformDefines::Vec4_MainLightPosition_NameInShader, Light::mMainLight->mPosition.v); 
				}
			}
		}
	}
	void DrawCall::SetUpLight(RenderPass*pass, Light*light) {
		pass->SetVec4Property(UniformDefines::Vec4_LightAmbient_NameInShader, light->mAmbientColor.v);
		pass->SetVec4Property(UniformDefines::Vec4_LightDiffuse_NameInShader, light->mDiffuseColor.v);
		pass->SetVec4Property(UniformDefines::Vec4_LightSpecular_NameInShader, light->mSpecularColor.v);
		pass->SetVec4Property(UniformDefines::Vec4_LightPos_NameInShader, light->mPosition.v);
		pass->SetVec4Property(UniformDefines::Vec4_LightSetting_NameInShader, light->mSetting.v);
		pass->SetVec4Property(UniformDefines::Vec4_LightSetting1_NameInShader, light->mSetting1.v);
	}
	void DrawCall::SetUpOnePassLights(RenderPass*pass) {
		GameObject*currentGO = (GameObject*)mGameObject;
		memset(Light::mLightEnableMask, 0, sizeof(int) * 8);
		auto iter = Light::mLights.begin();
		auto iterEnd = Light::mLights.end();
		int nLightIndex = 0;
		for (; iter != iterEnd; ++iter) {//max support 8 light,check if this material need light data
			Light*currentLight = *iter;
			if (currentLight->mOwner != nullptr && false == currentLight->mOwner->IsEnabled()) {
				continue;
			}
			if ((currentLight->mCullingMask&currentGO->mLayer) != 0) {
				Light::mLightEnableMask[nLightIndex] = 1;
				Light::mLightPos[nLightIndex] = currentLight->mPosition;
				Light::mLightAmbient[nLightIndex] = currentLight->mAmbientColor;
				Light::mLightDiffuse[nLightIndex] = currentLight->mDiffuseColor;
				Light::mLightSpecular[nLightIndex] = currentLight->mSpecularColor;
				Light::mLightSetting[nLightIndex] = currentLight->mSetting;
				Light::mLightSetting1[nLightIndex] = currentLight->mSetting1;
			}
			nLightIndex++;
		}
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightPos_NameInShader, 8, Light::mLightPos[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightAmbient_NameInShader, 8, Light::mLightAmbient[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightDiffuse_NameInShader, 8, Light::mLightDiffuse[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSpecular_NameInShader, 8, Light::mLightSpecular[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSetting_NameInShader, 8, Light::mLightSetting[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSetting1_NameInShader, 8, Light::mLightSetting1[0].v);
		pass->SetIntArrayProperty(UniformDefines::IntArray_LightEnabled_NameInShader, 8, Light::mLightEnableMask);
	}
	void DrawCall::SetUpCamera(RenderPass*pass) {
		if (pass->GetVec4Property(UniformDefines::Vec4_CameraWorldPos_NameInShader) != nullptr) {
			const Matrix4x4 & world_matrix = mCamera->mOwner->GetWorldMatrix();
			pass->SetVec4Property(UniformDefines::Vec4_CameraWorldPos_NameInShader,world_matrix.mData[12], world_matrix.mData[13], world_matrix.mData[14], 1.0f);
		}
	}
	void DrawCall::RenderNoLit(RenderPass*pass) {
		pass->ActiveRenderState();
		pass->UpdateUniforms(mCamera);
		pass->SetupStencilBuffer();
		if (mEBO == nullptr || mEBO->mIndexCount == 0) {
			mVBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		else {
			mEBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		pass->RestoreStencilBuffer();
	}
	void DrawCall::RenderUI(RenderPass*pass) {
		pass->ActiveRenderState();
		pass->UpdateUniforms(mCanvas);
		pass->SetupStencilBuffer();
		if (mEBO == nullptr || mEBO->mIndexCount == 0) {
			mVBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		else {
			mEBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		pass->RestoreStencilBuffer();
	}
	void DrawCall::RenderOnePass(RenderPass*pass) {
		SetUpDepthMap(pass);
		SetUpOnePassLights(pass);
		SetUpCamera(pass);
		pass->ActiveRenderState();
		pass->UpdateUniforms(mCamera);
		pass->SetupStencilBuffer();
		if (mEBO == nullptr || mEBO->mIndexCount == 0) {
			mVBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		else {
			mEBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		pass->RestoreStencilBuffer();
	}
	/*
	Base pass renders object with one per-pixel directional light and all SH/vertex lights. This pass also adds any lightmaps
	, ambient and emissive lighting from the shader . Directional light rendered in this pass can have Shadows. 
	Note that Lightmapped objects do not get illumination from SH lights.
	Note that when ¡°OnlyDirectional¡± pass flag is used in the shader, 
	then the forward base pass only renders main directional light, 
	ambient/lightprobe and lightmaps (SH and vertex lights are not included into pass data).
	*/
	void DrawCall::RenderForwardBasePass(RenderPass*pass) {
		SetUpDepthMap(pass);
		GameObject*currentGO = (GameObject*)mGameObject;
		if (Light::mMainLight != nullptr && (currentGO->mLayer&Light::mMainLight->mCullingMask) != 0) {
			if (Light::mMainLight->mOwner != nullptr && false == Light::mMainLight->mOwner->IsEnabled()) {
			}
			else {
				SetUpLight(pass, Light::mMainLight);
			}
		}
		SetUpCamera(pass);
		pass->ActiveRenderState();
		pass->SetupStencilBuffer();
		pass->UpdateUniforms(mCamera);
		if (mEBO == nullptr || mEBO->mIndexCount == 0) {
			mVBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		else {
			mEBO->Draw(pass->mRenderState.mPrimitiveType);
		}
		pass->RestoreStencilBuffer();
	}
	/*
	Additional passes are rendered for each additional per-pixel light that affect this object. 
	Lights in these passes by default do not have shadows 
	(so in result, Forward Rendering supports one directional light with shadows), 
	unless multi_compile_fwdadd_fullshadows variant shortcut is used.
	*/
	void DrawCall::RenderForwardAdditivePass(RenderPass*pass) {
		SetUpDepthMap(pass);
		SetUpCamera(pass);
		GameObject*currentGO = (GameObject*)mGameObject;
		for (auto iter = Light::mLights.begin(); iter != Light::mLights.end(); ++iter) {
			Light*current_light = *iter;
			if (current_light->mOwner != nullptr && false == current_light->mOwner->IsEnabled()) {
				continue;
			}
			if (current_light != Light::mMainLight&&(currentGO->mLayer&current_light->mCullingMask) != 0) {
				SetUpLight(pass, current_light);
				pass->ActiveRenderState();
				pass->SetupStencilBuffer();
				pass->UpdateUniforms(mCamera);
				if (mEBO == nullptr || mEBO->mIndexCount == 0) {
					mVBO->Draw(pass->mRenderState.mPrimitiveType);
				}
				else {
					mEBO->Draw(pass->mRenderState.mPrimitiveType);
				}
				pass->RestoreStencilBuffer();
			}
		}
	}
	void DrawCall::ForwardRendering() {
		mVBO->Active();
		for (auto iter = mMaterial->mRenderPasses.begin(); iter != mMaterial->mRenderPasses.end();++iter) {
			RenderPass*pass = *iter;
			switch (pass->mCatagory){
			case kRenderPassCatagoryNoLit:
				RenderNoLit(pass);
				break;
			case kRenderPassCatagoryOnePass:
				RenderOnePass(pass);
				break;
			case kRenderPassCatagoryForwardBase:
				RenderForwardBasePass(pass);
				break;
			case kRenderPassCatagoryForwardAdd:
				RenderForwardAdditivePass(pass);
				break;
			case kRenderPassCatagoryUI:
				RenderUI(pass);
				break;
			}
		}
	}
	void DrawCall::Rendering(){
		if (mMaterial != nullptr) {
			ForwardRendering();
		}
		if (mNext!=nullptr){
			Next<DrawCall>()->Rendering();
		}
	}
	void DrawCall::RenderingOpaque() {
		if (mMaterial != nullptr) {
			if (mMaterial->mRenderingQueue<kRenderingQueueTransparent){
				ForwardRendering();
			}
		}
		if (mNext != nullptr) {
			Next<DrawCall>()->RenderingOpaque();
		}
	}
	void DrawCall::RenderingTransparent() {
		if (mMaterial != nullptr) {
			if (mMaterial->mRenderingQueue >= kRenderingQueueTransparent) {
				ForwardRendering();
			}
		}
		if (mNext != nullptr) {
			Next<DrawCall>()->RenderingTransparent();
		}
	}
	void DrawCall::RenderingDepth() {
		if (mCamera->mDepthMaterial != nullptr && mVBO!=nullptr) {
			mVBO->Active();
			RenderPass*pass = mCamera->mDepthMaterial->GetPass(0);
			pass->SetMatrix4Property(UniformDefines::Matrix_M_NameInShader,mMaterial->GetMatrix4Property(UniformDefines::Matrix_M_NameInShader)->mValue.mData);
			pass->ActiveRenderState();
			pass->UpdateUniforms(mCamera);
			if (Light::mMainLight != nullptr) {
				mCamera->mDepthMaterial->SetMatrix4Property(UniformDefines::Matrix_MainLightProjection_NameInShader, Light::mMainLight->mProjection.mData);
				mCamera->mDepthMaterial->SetMatrix4Property(UniformDefines::Matrix_MainLightView_NameInShader, Light::mMainLight->mView.mData);
				mCamera->mDepthMaterial->SetVec4Property(UniformDefines::Vec4_MainLightPosition_NameInShader, Light::mMainLight->mPosition.v);
			}
			if (mEBO == nullptr || mEBO->mIndexCount == 0) {
				mVBO->Draw(pass->mRenderState.mPrimitiveType);
			}
			else {
				mEBO->Draw(pass->mRenderState.mPrimitiveType);
			}
		}
		if (mNext != nullptr) {
			Next<DrawCall>()->RenderingDepth();
		}
	}
	static void AppendSortedRenderQueue(DrawCall*head, DrawCall*new_node) {
		DrawCall*current_node = head;
		DrawCall*prev_node = current_node;
		while (current_node != nullptr) {
			if (new_node->mDistanceToCamera <= current_node->mDistanceToCamera) {
				new_node->InsertAfter(prev_node);
				prev_node = nullptr;
				break;
			}
			prev_node = current_node;
			current_node = current_node->Next<DrawCall>();
		}
		if (prev_node != nullptr) {
			prev_node->Append(new_node);
		}
	}
	void RenderQueue::AppendUIDrawCall(DrawCall*dc) {
		dc->mCamera = mCamera;
		if (mUIDrawCall==nullptr){
			mUIDrawCall = dc;
		} else {
			mLastUIDrawCall->Append(dc);
		}
		mLastUIDrawCall = dc;
	}
	void RenderQueue::AppendOpaqueDrawCall(DrawCall*dc) {
		dc->mCamera = mCamera;
		if (mOpaqueDrawCall == nullptr) {
			mOpaqueDrawCall = dc;
		}
		else {
			AppendSortedRenderQueue(mOpaqueDrawCall, dc);
		}
	}
	void RenderQueue::AppendTransparentDrawCall(DrawCall*dc) {
		dc->mCamera = mCamera;
		if (mTransparentDrawCall == nullptr) {
			mTransparentDrawCall = dc;
		}
		else {
			AppendSortedRenderQueue(mTransparentDrawCall, dc);
		}
	}
	void RenderQueue::AppendDepthDrawCall(DrawCall*dc) {
		dc->mCamera = mCamera;
		if (mDepthDrawCall == nullptr) {
			mDepthDrawCall = dc;
		}
		else {
			AppendSortedRenderQueue(mDepthDrawCall, dc);
		}
	}
	void RenderQueue::RenderUI() {
		DrawCall*current_node = mUIDrawCall;
		while (current_node != nullptr) {
			if (current_node->mMaterial != nullptr) {
				current_node->ForwardRendering();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}
	void RenderQueue::RenderUIEx() {
		DrawCall*current_node = mUIDrawCall;
		while (current_node != nullptr) {
			if (current_node->mMaterial != nullptr) {
				current_node->mCanvas = mCanvas;
				current_node->ForwardRendering();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}
	void RenderQueue::RenderDepth() {
		DrawCall*current_node = mDepthDrawCall;
		while (current_node != nullptr) {
			if (current_node->mMaterial != nullptr) {
				current_node->RenderingDepth();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}
	void RenderQueue::RenderOpaque() {
		DrawCall*current_node = mOpaqueDrawCall;
		while (current_node!=nullptr){
			if (current_node->mMaterial != nullptr) {
				current_node->ForwardRendering();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}
	void RenderQueue::RenderTransparent() {
		DrawCall*current_node = mLastTransparentDrawCall;
		while (current_node != nullptr) {
			if (current_node->mMaterial != nullptr) {
				current_node->ForwardRendering();
			}
			current_node = current_node->Prev<DrawCall>();
		}
	}
}
