#include "RenderPass.h"
#include "RenderState.h"
namespace Alice{
	RenderPass::RenderPass() :Object(1),
		mShader(nullptr){
		mbUseStencilMask = false;
		mbWriteStencilMask = false;
		mCatagory = kRenderPassCatagoryOnePass;
	}
	RenderPass::~RenderPass() {
		ClearUniformUpdaters();
	}
	void RenderPass::operator=(const RenderPass&r) {
		mShader = r.mShader.mPtr;
	}
	RenderPass*RenderPass::Clone() const {
		RenderPass*pass = new RenderPass;
		pass->mShader = mShader.mPtr;
		pass->mbUseStencilMask = mbUseStencilMask;
		pass->mbWriteStencilMask = mbWriteStencilMask;
		pass->mCatagory = mCatagory;
		pass->mRenderState = mRenderState;
		for (auto iter = mProperties.begin(); iter != mProperties.end(); iter++) {
			pass->mProperties.insert(std::pair<std::string, MaterialProperty*>(iter->first, iter->second->Clone()));
		}
		for (auto iter = mPropertyUpdaters.begin(); iter != mPropertyUpdaters.end(); iter++) {
			UniformUpdater*src_updater = iter->second;
			UniformUpdater*updater = new UniformUpdater(src_updater);
			updater->mUserData = pass->mProperties[iter->first];
			pass->AddUniformUpdater(updater);
			pass->mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(iter->first, updater));
		}
		return pass;
	}
	void RenderPass::SetShader(Shader*shader) {
		mShader = shader;
		ClearUniformUpdaters();
		for (auto iter = mShader->mActiveUniforms.begin(); iter != mShader->mActiveUniforms.end(); ++iter) {
			ActiveUniform(iter->first.c_str(), iter->second);
		}
		auto iter= mProperties.find("U_Color");
		if (iter!=mProperties.end()){
			((UniformVector4Property*)(iter->second))->mValue.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}
		iter = mProperties.find(UniformDefines::Vec4_ColorScale_NameInShader);
		if (iter != mProperties.end()) {
			((UniformVector4Property*)(iter->second))->mValue.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	void RenderPass::ActiveUniform(const char *name, const ShaderUniformDefine * sud) {
		switch (sud->mType) {
		case GL_FLOAT:
			ActiveUniformFloat(name,sud);
			break;
		case GL_FLOAT_VEC2:
			ActiveUniformVec2(name, sud);
			break;
		case GL_FLOAT_VEC3:
			ActiveUniformVec3(name, sud);
			break;
		case GL_FLOAT_VEC4:
			if (sud->mSize == 1) {
				ActiveUniformVec4(name, sud);
			}
			else {
				ActiveUniformVec4Array(name, sud);
			}
			break;
		case GL_FLOAT_MAT3:
			ActiveUniformMat3(name, sud);
			break;
		case GL_FLOAT_MAT4:
			ActiveUniformMat4(name, sud);
			break;
		case GL_INT:
			if (sud->mSize == 1) {
			}
			else {
				ActiveUniformIntArray(name, sud);
			}
			break;
		case GL_SAMPLER_2D:
			ActiveUniformSampler2D(name, sud);
			break;
		case GL_SAMPLER_CUBE:
			ActiveUniformSamplerCube(name, sud);
			break;
		}
	}void RenderPass::ActiveUniformFloat(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformFloatProperty*prop = new (kMemMaterialId)UniformFloatProperty();
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateFloat, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformVec2(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformVector2Property*prop = new (kMemMaterialId)UniformVector2Property();
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec2, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformVec3(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformVector3Property*prop = new (kMemMaterialId)UniformVector3Property();
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec3, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformVec4(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformVector4Property*prop = new (kMemMaterialId)UniformVector4Property();
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = nullptr;
			if (strcmp(name, UniformDefines::Vec4_Canvas_NameInShader) == 0) {
				updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateCanvas, sud->mLocation, material_property);
			}
			else {
				updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec4, sud->mLocation, material_property);
			}
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformSampler2D(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			TextureUnitProperty*prop = new (kMemMaterialId)TextureUnitProperty(kUniformTypeSampler2D);
			prop->mName = name;
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateTextureUnit, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformSamplerCube(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			TextureUnitProperty*prop = new (kMemMaterialId)TextureUnitProperty(kUniformTypeSamplerCube);
			prop->mName = name;
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateTextureUnit, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformIntArray(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformIntArrayProperty*prop = new (kMemMaterialId)UniformIntArrayProperty();
			prop->mValue = nullptr;
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateIntArray, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformVec4Array(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformVector4ArrayProperty*prop = new (kMemMaterialId)UniformVector4ArrayProperty();
			prop->mValue = nullptr;
			prop->mLocation = sud->mLocation;
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec4Array, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformMat3(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformMatrix3Property*prop = new (kMemMaterialId)UniformMatrix3Property();
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			prop->mLocation = sud->mLocation;
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateMat3, sud->mLocation, material_property);
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	void RenderPass::ActiveUniformMat4(const char*name, const ShaderUniformDefine * sud) {
		auto iter_property = mProperties.find(name);
		MaterialProperty*material_property = nullptr;
		if (iter_property == mProperties.end()) {
			UniformMatrix4Property*prop = new (kMemMaterialId)UniformMatrix4Property();
			mProperties.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			prop->mLocation = sud->mLocation;
			material_property = prop;
		}
		else {
			material_property = iter_property->second;
		}
		auto iter_updater = mPropertyUpdaters.find(name);
		if (iter_updater == mPropertyUpdaters.end()) {
			UniformUpdater*updater = nullptr;
			if (strcmp(name, UniformDefines::Matrix_P_NameInShader) == 0) {
				updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateProjectionMatrix, sud->mLocation, material_property);
			}
			else if (strcmp(name, UniformDefines::Matrix_V_NameInShader) == 0) {
				updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateViewMatrix, sud->mLocation, material_property);
			}
			else {
				updater = new (kMemMaterialId)UniformUpdater(UniformUpdater::UpdateMat4, sud->mLocation, material_property);
			}
			mPropertyUpdaters.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else {
			AddUniformUpdater(iter_updater->second);
		}
	}
	UniformMatrix4Property* RenderPass::GetMatrix4Property(const char*uniformName) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeMatrix4) {
				return (UniformMatrix4Property*)iter->second;
			}
		}
		return nullptr;
	}

	UniformVector4Property* RenderPass::GetVec4Property(const char*uniformName) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeVec4) {
				return (UniformVector4Property*)iter->second;
			}
		}
		return nullptr;
	}

	TextureUnitProperty* RenderPass::GetTextureProperty(const char*uniformName) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeSampler2D) {
				return (TextureUnitProperty*)iter->second;
			}
		}
		return nullptr;
	}
	void RenderPass::SetTextureProperty(const char*uniformName, TextureUnit*texture) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeSampler2D) {
				((TextureUnitProperty*)iter->second)->mValue = texture;
			}
		}
	}

	void RenderPass::SetTextureCubeProperty(const char*uniformName, TextureCube*texture) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeSamplerCube) {
				((TextureUnitProperty*)iter->second)->mValue = texture;
			}
		}
	}
	void RenderPass::SetFloatProperty(const char*uniformName, float v) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeFloat) {
				((UniformFloatProperty*)iter->second)->mValue=v;
			}
		}
	}
	void RenderPass::SetVec2Property(const char*uniformName, float x, float y) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeVec2) {
				((UniformVector2Property*)iter->second)->mValue.Set(x, y);
			}
		}
	}
	void RenderPass::SetVec3Property(const char*uniformName, float x, float y, float z) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeVec3) {
				((UniformVector3Property*)iter->second)->mValue.Set(x, y, z);
			}
		}
	}
	void RenderPass::SetVec4Property(const char*uniformName, float x, float y, float z, float w) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeVec4) {
				((UniformVector4Property*)iter->second)->mValue.Set(x, y, z, w);
			}
		}
	}
	void RenderPass::SetVec4Property(const char*uniformName, float * v) {
		SetVec4Property(uniformName, v[0], v[1], v[2], v[3]);
	}
	void RenderPass::SetVec4Property(const char*uniformName, const Serializer::Vector4f&value) {
		SetVec4Property(uniformName, value.x(), value.y(), value.z(), value.w());
	}
	void RenderPass::SetVec4ArrayProperty(const char*uniformName, int size, float*v) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeVector4Array) {
				((UniformVector4ArrayProperty*)iter->second)->mSize = size;
				((UniformVector4ArrayProperty*)iter->second)->mValue = v;
			}
		}
	}
	void RenderPass::SetIntArrayProperty(const char*uniformName, int size, int*v) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeIntArray) {
				((UniformIntArrayProperty*)iter->second)->mSize = size;
				((UniformIntArrayProperty*)iter->second)->mValue = v;
			}
		}
	}


	void RenderPass::SetMatrix4Property(const char*uniformName, const float *v) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeMatrix4) {
				memcpy(((UniformMatrix4Property*)iter->second)->mValue.mData, v, sizeof(float) * 16);
			}
		}
	}
	void RenderPass::SetMatrix3Property(const char*uniformName, float *v) {
		auto iter = mProperties.find(uniformName);
		if (iter != mProperties.end()) {
			if (iter->second->mValueType == kUniformTypeMatrix3) {
				memcpy(((UniformMatrix3Property*)iter->second)->mValue.mData, v, sizeof(float) * 9);
			}
		}
	}
	void RenderPass::UpdateUniforms(void * camera) {
		int nTextureSlot = 0;
		for (auto iter = mActiveUniformList.begin(); iter != mActiveUniformList.end(); ++iter) {
			UniformUpdater*updater = *iter;
			if (updater->mFoo != nullptr) {
				if (updater->mFoo == UniformUpdater::UpdateTextureUnit) {
					TextureUnitProperty*prop = (TextureUnitProperty*)updater->mUserData;
					prop->mSlot = nTextureSlot++;
				}
				updater->mFoo(updater->mLocation, camera, updater->mUserData);
			}
		}
	}
	void RenderPass::ClearUniformUpdaters() {
		if (!mProperties.empty()) {
			for (auto iter = mProperties.begin(); iter != mProperties.end(); ++iter) {
				delete iter->second;
			}
		}
		if (!mPropertyUpdaters.empty()) {
			for (auto iter = mPropertyUpdaters.begin(); iter != mPropertyUpdaters.end(); ++iter) {
				delete iter->second;
			}
		}
		mProperties.clear();
		mPropertyUpdaters.clear();
		mActiveUniformList.clear();
	}
	void RenderPass::AddUniformUpdater(UniformUpdater*updater) {
		mActiveUniformList.insert(updater);
	}
	void RenderPass::ActiveRenderState() {
		mShader->mGPUProgram->MakeAsCurrentShader();
		GlobalRenderState::EnableBlend(mRenderState.mbBlend);
		if (mRenderState.mbBlend) {
			GlobalRenderState::BlendFunc(mRenderState.mSRCBlendFunc, mRenderState.mDSTBlendFunc);
		}
		GlobalRenderState::EnableCull(mRenderState.mbCull);
		GlobalRenderState::CullFace(mRenderState.mCullFace);
		GlobalRenderState::EnableZWrite(mRenderState.mbWriteZ);
		GlobalRenderState::EnableDepthTest(mRenderState.mbZTest);
		GlobalRenderState::ZTest(mRenderState.mZTestFunction);
		GlobalRenderState::ZOffset(mRenderState.mOffsetFactor, mRenderState.mOffsetUnit);
		GlobalRenderState::EnableAlphaTest(mRenderState.mbAlphaTest);
		if (mRenderState.mbAlphaTest){
			GlobalRenderState::AlphaTest(mRenderState.mAlphaTestFunction, mRenderState.mAlphaTestValue);
		}
		GlobalRenderState::EnablePointSprite(mRenderState.mbEnablePointSprite);
		GlobalRenderState::PolygonMode(mRenderState.mPolygonMode);
	}
	void RenderPass::SetupStencilBuffer() {
		if (mbWriteStencilMask) {
			GlobalRenderState::ClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glColorMask(false, false, false, false);
		}
		else if (mbUseStencilMask) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		else {
			glDisable(GL_STENCIL_TEST);
		}
	}
	void RenderPass::RestoreStencilBuffer() {
		if (mbWriteStencilMask) {
			glDisable(GL_STENCIL_TEST);
			glColorMask(true, true, true, true);
		}
		else if (mbUseStencilMask) {
			glDisable(GL_STENCIL_TEST);
		}
	}
}
