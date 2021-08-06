#include "Material.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ShaderResource.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Render/RenderState.h"
namespace Alice{
	Material* Material::mDefault2DMaterial = nullptr;
	UniformVector4Property Material::mTime;
	UniformVector4Property Material::mSinTime;
	UniformVector4Property Material::mCosTime;
	Material::Material() :Object(1),
		mbSharedMaterial(true){
		mRenderingQueue = kRenderingQueueOpaque;
		mbCastShadow = true;
		mbReceiveShadow = false;
	}
	void Material::SetStencilBuffer() {
	}
	void Material::RestoreStencilBuffer() {
	}
	void Material::EnableBlend(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mbBlend = enable;
		}
	}
	void Material::EnableZTest(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mbZTest = enable;
		}
	}
	void Material::EnableZWrite(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mbWriteZ = enable;
		}
	}
	void Material::EnableCull(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mbCull = enable;
		}
	}
	void Material::EnablePointSprite(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mbEnablePointSprite = enable;
		}
	}
	void Material::EnableWriteStencilMask(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mbWriteStencilMask = enable;
		}
	}
	void Material::EnableUseStencilMask(bool enable) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mbUseStencilMask = enable;
		}
	}
	void Material::ZTestFunction(int func) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mZTestFunction = func;
		}
	}
	void Material::ZOffset(float factor, float unit) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mOffsetFactor = factor;
			(*iter)->mRenderState.mOffsetUnit = unit;
		}
	}
	void Material::SetCullFace(int face) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mCullFace = face;
		}
	}
	void Material::SetPolygonMode(int mode) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mPolygonMode = mode;
		}
	}
	void Material::SetPrimitive(int primitive) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mPrimitiveType = primitive;
		}
	}
	void Material::BlendFunc(int src, int dst) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->mRenderState.mSRCBlendFunc = src;
			(*iter)->mRenderState.mDSTBlendFunc = dst;
		}
	}
	void Material::AddPass(RenderPass * pass) {
		mRenderPasses.push_back(pass);
	}
	void Material::RemovePass(int index) {
		int i = 0;
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			if (index == i) {
				delete *iter;
				mRenderPasses.erase(iter);
				break;
			}
			++i;
		}
	}
	RenderPass * Material::GetPass(int index) {
		int i = 0;
		for (auto iter=mRenderPasses.begin();iter!=mRenderPasses.end();++iter){
			if (index==i){
				return *iter;
			}
			++i;
		}
		return nullptr;
	}
	Material::~Material(){
		CleanUpProperties();
	}
	bool Material::operator ==(const Material & r){
		return false;
	}
	bool Material::operator ==(const std::unordered_map<std::string, MaterialProperty*> & mpm){
		return false;
	}
	void Material::operator=(const Material&src){
		for (auto iter=src.mRenderPasses.begin();iter!= src.mRenderPasses.end();++iter){
			AddPass((*iter)->Clone());
		}
		mbReceiveShadow = src.mbReceiveShadow;
		mName = src.mName.mText;
		mbSharedMaterial = false;
		if (!mName.EndWith("(Clone)")){
			mName << "(Clone)";
		}
	}
	UniformMatrix4Property* Material::GetMatrix4Property(const char*uniformName) {
		for (auto iter=mRenderPasses.begin();iter!=mRenderPasses.end();++iter){
			UniformMatrix4Property* prop = (*iter)->GetMatrix4Property(uniformName);
			if (prop!=nullptr){
				return prop;
			}
		}
		return nullptr;
	}
	UniformVector4Property* Material::GetVec4Property(const char*uniformName) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			UniformVector4Property* prop = (*iter)->GetVec4Property(uniformName);
			if (prop != nullptr) {
				return prop;
			}
		}
		return nullptr;
	}
	TextureUnitProperty* Material::GetTextureProperty(const char*uniformName) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			TextureUnitProperty* prop = (*iter)->GetTextureProperty(uniformName);
			if (prop != nullptr) {
				return prop;
			}
		}
		return nullptr;
	}
	void Material::SetTextureProperty(const char*uniformName, TextureUnit*texture) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetTextureProperty(uniformName,texture);
		}
	}

	void Material::SetTextureCubeProperty(const char*uniformName, TextureCube*texture) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetTextureCubeProperty(uniformName, texture);
		}
	}
	void Material::SetVec4Property(const char*uniformName, float x, float y, float z, float w) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetVec4Property(uniformName, x,y,z,w);
		}
	}
	void Material::SetVec4Property(const char*uniformName, float * v){
		SetVec4Property(uniformName, v[0], v[1], v[2], v[3]);
	}
	void Material::SetVec4Property(const char*uniformName, const Serializer::Vector4f&value) {
		SetVec4Property(uniformName, value.x(), value.y(), value.z(), value.w());
	}
	void Material::SetVec4ArrayProperty(const char*uniformName, int size, float*v) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetVec4ArrayProperty(uniformName, size, v);
		}
	}
	void Material::SetIntArrayProperty(const char*uniformName, int size, int*v) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetIntArrayProperty(uniformName, size, v);
		}
	}
	void Material::SetMatrix4Property(const char*uniformName, const float *v) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetMatrix4Property(uniformName, v);
		}
	}
	void Material::SetMatrix3Property(const char*uniformName, float *v) {
		for (auto iter = mRenderPasses.begin(); iter != mRenderPasses.end(); ++iter) {
			(*iter)->SetMatrix3Property(uniformName, v);
		}
	}
	//path -> a set of materials : the original one and its clones
	std::unordered_map<std::string, Material*> Material::mCachedMaterials;
	Material*Material::GetCachedMaterial(const char*matPath){
		Material*ret = nullptr;
		auto iter = mCachedMaterials.find(matPath);
		if (iter != mCachedMaterials.end()){
			//if the object change other property then clone in other method
			ret = iter->second;
		}
		return ret;
	}

	void Material::UnloadMaterial(const char*name){
		Alice::Resource::Unload(name);
		auto iter = mCachedMaterials.find(name);
		if (iter != mCachedMaterials.end()){
			Material*mat = iter->second;
			delete mat;
			mCachedMaterials.erase(iter);
		}
	}
	void Material::CleanUpProperties(){
	}
	static Material* CreateMaterialWithSerializedData(const char*name, Serializer::Material*material_serialized_data) {
		return nullptr;
	}

	Material*Material::LoadMaterial(const char*name){
		return nullptr;
	}
}
