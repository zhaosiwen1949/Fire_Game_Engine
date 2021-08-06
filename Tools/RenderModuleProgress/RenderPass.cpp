#include "RenderPass.h"
#include "utils.h"
#include "VertexData.h"
void RenderPass::Bind(Camera*camera) {
	glUseProgram(mShader->mProgram);
	GloblalRenderState::SetBlendState(mRenderState.mbEnableBlend);
	GloblalRenderState::SetBlendFunc(mRenderState.mSrcBlendFunc, mRenderState.mDstBlendFunc);
	GloblalRenderState::SetDepthTestState(mRenderState.mbEnableDepthTest);
	GloblalRenderState::SetDepthMask(mRenderState.mbWriteDepthBuffer);
	if (mShader->mAttributes != nullptr) {
		mShader->mAttributes->Active();
	}
	UniformUpdater*uniform_updater = mUniforms;
	int slot_index = 0;
	while (uniform_updater != nullptr) {
		if (uniform_updater->mUpdateFunction == UniformUpdater::UpdateSampler2D) {
			glActiveTexture(GL_TEXTURE0 + slot_index);
			MaterialPropertySampler2D* prop = (MaterialPropertySampler2D*)uniform_updater->mTargetProperty;
			glBindTexture(GL_TEXTURE_2D,prop->mTextureName);
			glUniform1i(uniform_updater->mLocation, slot_index);
			slot_index++;
		}
		else {
			uniform_updater->mUpdateFunction(uniform_updater->mLocation, camera, uniform_updater->mTargetProperty);
		}
		uniform_updater = uniform_updater->Next<UniformUpdater>();
	}
}
void RenderPass::SetMatrix4(const char *uniform_name, float*mat4) {
	auto iter = mProperties.find(uniform_name);
	if (iter!=mProperties.end()){
		((MaterialPropertyMatrix4*)iter->second)->mPropertyValue = mat4;
	}
}
void RenderPass::SetVec4(const char *uniform_name, float*ptr) {
	auto iter = mProperties.find(uniform_name);
	if (iter != mProperties.end()) {
		((MaterialPropertyVec4*)iter->second)->mPropertyValue = ptr;
	}
}
void RenderPass::SetSampler2D(const char* uniform_name, GLuint texture_name) {
	auto iter = mProperties.find(uniform_name);
	if (iter != mProperties.end()) {
		((MaterialPropertySampler2D*)iter->second)->mTextureName = texture_name;
	}
}
void RenderPass::SetShader(Shader*shader) {
	mShader = shader;
	mUniforms = nullptr;
	UniformUpdater*current_uniform_updater = shader->mUniforms;
	while (current_uniform_updater != nullptr) {
		UniformUpdater*uniform_updater = current_uniform_updater->Clone();
		AppendUniformUpdater(uniform_updater);
		if (uniform_updater->mTargetProperty != nullptr) {
			mProperties.insert(std::pair<std::string, MaterialProperty*>(uniform_updater->mTargetProperty->mName, uniform_updater->mTargetProperty));
		}
		current_uniform_updater = current_uniform_updater->Next<UniformUpdater>();
	}
}
void RenderPass::AppendUniformUpdater(UniformUpdater*uniform_updater) {
	if (mUniforms == nullptr) {
		mUniforms = uniform_updater;
	}
	else {
		mUniforms->PushBack(uniform_updater);
	}
}