#include "FontResource.h"
#include "ResourceManager.h"
namespace Alice{
	FontResource*FontResource::LoadFontResource(Serializer::RuntimeAsset*asset){
		FontResource*fontResource = new (kMemDefaultId)FontResource;
		fontResource->mName = asset->guid();
		return fontResource;
	}
	FontResource*FontResource::CreateFontResourceWithRawData(const char*path, Data&data) {
		FontResource*fontResource = new (kMemDefaultId)FontResource;
		Serializer::RuntimeAsset *asset = new (kMemAssetBundleId)Serializer::RuntimeAsset;
		fontResource->mName = path;
		asset->set_rawdata(data.mData, data.mDataLen);
		asset->set_type(15);
		asset->set_guid(path);
		fontResource->mAssetBundle = asset;
		return fontResource;
	}
}