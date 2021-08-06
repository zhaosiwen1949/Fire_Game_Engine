#pragma once
#include "AliceResource.h"
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "AliceData.h"
#include "AliceResourceType.h"
#include "ResourceManager.h"
#include "Runtime/Debugger/log.h"
#include "Runtime/Serializer/Assets.serializer.h"
#include <unordered_map>
namespace Alice{
	class Resource :public Object{
	public:
		Resource();
		virtual ~Resource();
		ResourceType mType;
		///Runtime Asset package,compressed data
		Serializer::RuntimeAsset*mAssetBundle;
		///Runtime Serializer::TextureInfo or some other,resource raw data
		google::protobuf::MessageLite*mResourceData;
		void Unload();
		template<typename T>
		T* RuntimeData(){
			return (T*)mResourceData;
		}
	public:
		static void Init();
		static bool LoadAssetList(const char *path);
		static void UnloadAssetList(const char *path);
		static void AddResource(const char*name,const char*path,const char*userPath=nullptr);
		static void RemoveResource(const char*name, const char*userPath = nullptr);
		static void Unload(const char*name);
		static Resource * LoadAliceAsset(const char*path, Data&data);
		static Resource * LoadRawData(const char * path, Data&data);
		static Resource * LoadAssetWithUserPath(const char*path);//user path maybe direct path or need to change to engine path
		//load resource from outer
		static Resource * LoadOuter(const char * path);
		//guid to resource real path map
		static const char* GetResourcePath(const char*guid);
		//assets folder relative path to resource real path map
		static const char* GetResourceGUIDViaUserName(const char*path);
		static std::unordered_map<std::string, std::string> mAssetsIDToPathMap;
		static std::unordered_map<std::string, std::string> mPathToAssetsIDMap;
		static std::unordered_map<std::string, Resource*> mCachedResources;
		static std::unordered_map<AliceSInt32, Resource*(*)(Serializer::RuntimeAsset*)> mAssetLoader;
		template<typename T>
		static T*LoadResource(const char*path) {
			if (mCachedResources.find(path) != mCachedResources.end()) {//if is cached
				return (T*)mCachedResources[path];
			}
			T*ret = nullptr;
			Alice::Data data;
			if (ResourceManager::LoadData(path,data)) {
				//try to decode data as asset bundle data
				ret=(T*)LoadAliceAsset(path, data);
				if (ret!=nullptr){
					return ret;
				}
				//try to decode data as raw data
				return (T*)LoadRawData(path, data);
			}
			return ret;
		}
	public:
		DECLEAR_ALICE_CLASS(Resource)
	};
}
