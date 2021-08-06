#include "AliceResource.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Serializer/Assets.serializer.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"
#include "Runtime/Debugger/Debugger.h"
#include "AudioResource.h"
#include "TextureResource.h"
#include "FontResource.h"

namespace Alice
{
	std::unordered_map<std::string, std::string> Resource::mAssetsIDToPathMap;
	std::unordered_map<std::string, std::string> Resource::mPathToAssetsIDMap;
	std::unordered_map<std::string, Resource*> Resource::mCachedResources;
	std::unordered_map<AliceSInt32, Resource*(*)(Serializer::RuntimeAsset*)> Resource::mAssetLoader;
	Resource::Resource():mType(ResourceTypeNone),mAssetBundle(nullptr),mResourceData(nullptr),Object(1)
	{
	}

	Resource::~Resource(){
		if (mResourceData!=nullptr){
			delete mResourceData;
		}
		if (mAssetBundle!=nullptr){
			delete mAssetBundle;
		}
        //Info("delete %s",mName.mText);
		std::unordered_map<std::string, Resource*>::iterator iter = mCachedResources.find(mName.mText);
		if (iter != mCachedResources.end()){
            //Info("delete from cached resource list %s",mName.mText);
			mCachedResources.erase(iter);
		}
	}

	void Resource::Unload(const char*name){
		std::unordered_map<std::string, Resource*>::iterator iter = mCachedResources.find(name);
		if (iter!=mCachedResources.end()){
			Resource*res = iter->second;
            mCachedResources.erase(iter);
			delete res;
		}
	}

	Resource*LoadImageResource(Serializer::RuntimeAsset*asset)
	{
		return TextureResource::LoadTexture(asset);
	}

	Resource*LoadShaderResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	Resource*LoadMaterialResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	Resource*LoadAudioResource(Serializer::RuntimeAsset*asset)
	{
		return AudioResource::LoadAudioResource(asset);
	}

	Resource*LoadFontResource(Serializer::RuntimeAsset*asset)
	{
		return FontResource::LoadFontResource(asset);
	}

	Resource*LoadPrefabResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	Resource*LoadSceneResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	Resource*LoadAnimationResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	Resource*LoadBytesResource(Serializer::RuntimeAsset*asset){
		Resource*res = new (kMemDefaultId)Resource;
		res->mAssetBundle = asset;
		res->mType = (ResourceType)asset->type();
		res->mName = asset->guid();
		return res;
	}

	Resource*LoadScriptResource(Serializer::RuntimeAsset*asset)
	{
		return nullptr;
	}

	void Resource::Init()
	{
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeJPGImage, LoadImageResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypePNGImage, LoadImageResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeMaterial, LoadMaterialResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeShader, LoadShaderResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeMP3, LoadAudioResource));

		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeFont, LoadFontResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypePrefab, LoadPrefabResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeScene, LoadSceneResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeAnimation, LoadAnimationResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeBytesData, LoadBytesResource));
		mAssetLoader.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*)>(ResourceTypeScript, LoadScriptResource));
	}

	void Resource::Unload(){

	}

	Resource * Resource::LoadOuter(const char*path)
	{
		Data data;
		Resource* ret = nullptr;
		if (ResourceManager::LoadOuterData(path,data))
		{
			if (StringUtils::EndWithI(path, ".asset"))
			{
				ret = LoadAliceAsset(path,data);
			}
			else
			{
				ret = LoadRawData(path,data);
			}
		}
		else
		{
			Error("Resource::LoadOuter fail %s",path);
		}
		return ret;
	}

	Resource * Resource::LoadRawData(const char*path, Data&data){
		Resource* ret = nullptr;
		if (StringUtils::EndWithI(path,".png")){
			ret=TextureResource::CreateTextureResourceFromRawData(path,data);
		}else if (StringUtils::EndWithI(path, ".jpg")) {
			ret = TextureResource::CreateTextureResourceFromRawData(path, data);
		}else if (StringUtils::EndWithI(path, ".bmp")) {
			ret = TextureResource::CreateTextureResourceFromRawData(path, data);
		}else if (StringUtils::EndWithI(path,".fbx")){
			//
		}else if (StringUtils::EndWithI(path,".mp3")){
			ret = AudioResource::CreateAudioResourceWithRawData(path,data);
		}
		else if (StringUtils::EndWithI(path, ".ttf")) {
			ret = FontResource::CreateFontResourceWithRawData(path, data);
		}
		if (ret!=nullptr){
			mCachedResources.insert(std::pair<std::string, Resource*>(path, ret));
		}
		return ret;
	}

	Resource * Resource::LoadAssetWithUserPath(const char*path){
		std::unordered_map<std::string, Resource*>::iterator iter = mCachedResources.find(path);
		if (iter != mCachedResources.end()){
			iter->second->retain();
			return iter->second;
		}
		Data data;
		FixedString realPath(256);
		if (ResourceManager::LoadData(path, data,&realPath)) {
			Serializer::RuntimeAsset *asset = new (kMemAssetBundleId)Serializer::RuntimeAsset;
			if (asset->ParseFromArray(data.mData, data.mDataLen)) {
				Resource*res = new (kMemDefaultId)Resource;
				res->mType = (ResourceType)asset->type();
				res->mName = realPath.mText;
				res->mAssetBundle = asset;
				mCachedResources.insert(std::pair<std::string, Resource*>(path, res));
				return res;
			} else {
				delete asset;
			}
		} else {
			Error("load file fail %s", path);
		}
		return nullptr;
	}
	static Serializer::RuntimeAsset* DecodeRuntimeAsset(Data&data) {
		Serializer::RuntimeAsset *asset = new (kMemAssetBundleId)Serializer::RuntimeAsset;
		if (asset->ParseFromArray(data.mData, data.mDataLen)) {
			return asset;
		}else {
			delete asset;
		}
		return nullptr;
	}

	Resource * Resource::LoadAliceAsset(const char*path, Data&data){
		Serializer::RuntimeAsset *asset = DecodeRuntimeAsset(data);
		if (asset!=nullptr){
			std::unordered_map<AliceSInt32, Resource*(*)(Serializer::RuntimeAsset*)>::iterator iterDecoder = mAssetLoader.find(asset->type());
			if (iterDecoder != mAssetLoader.end()){
				Resource*res = iterDecoder->second(asset);
				if (res != nullptr){
					res->mAssetBundle = asset;
					res->mName = path;
					mCachedResources.insert(std::pair<std::string, Resource*>(path, res));
					return res;
				}else{
					Error("cannot decode of %d %s", asset->type(), path);
					delete asset;
				}
			}else{
				Error("cannot find decoder of %d %s", asset->type(), path);
				delete asset;
			}
			return nullptr;
        }else{
            //Error("LoadAliceAsset:cannot decode %s",path);
        }
		return nullptr;
	}
	void Resource::AddResource(const char*name, const char*path, const char*userPath){
		std::unordered_map<std::string, std::string>::iterator iter = mAssetsIDToPathMap.find(name);
		if (iter==mAssetsIDToPathMap.end()){
			mAssetsIDToPathMap.insert(std::pair<std::string,std::string>(name,path));
		}
		if (userPath!=nullptr){
			std::unordered_map<std::string, std::string>::iterator iter = mPathToAssetsIDMap.find(userPath);
			if (iter == mPathToAssetsIDMap.end()){
				mPathToAssetsIDMap.insert(std::pair<std::string, std::string>(userPath, name));
			}
		}
	}
	void Resource::RemoveResource(const char*name, const char*userPath){
		std::unordered_map<std::string, std::string>::iterator iter = mAssetsIDToPathMap.find(name);
		if (iter != mAssetsIDToPathMap.end()){
			mAssetsIDToPathMap.erase(iter);
		}
		if (userPath!=nullptr){
			std::unordered_map<std::string, std::string>::iterator iterX = mPathToAssetsIDMap.find(userPath);
			if (iterX != mPathToAssetsIDMap.end()){
				mPathToAssetsIDMap.erase(iterX);
			}
		}
	}

	const char* Resource::GetResourcePath(const char*guid){
		std::unordered_map<std::string, std::string>::iterator iter = mAssetsIDToPathMap.find(guid);
		if (iter != mAssetsIDToPathMap.end()){
			return iter->second.c_str();
		}
		return nullptr;
	}

	const char* Resource::GetResourceGUIDViaUserName(const char*path){
		std::unordered_map<std::string, std::string>::iterator iter = mPathToAssetsIDMap.find(path);
		if (iter != mPathToAssetsIDMap.end()){
			return iter->second.c_str();
		}
		return nullptr;
	}

	bool Resource::LoadAssetList(const char *path) {
		return false;
	}

	void Resource::UnloadAssetList(const char *path) {
	}
}
