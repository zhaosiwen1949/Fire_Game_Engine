#include "ImageResource.h"
#include "ResourceManager.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "FileSystem.h"
#include "Runtime/Debugger/Debugger.h"
namespace Alice{
	ImageResource::ImageResource(){
		mType = ResourceTypeImage;
		mWidth = 0;
		mHeight = 0;
		mBPP = 0;
	}
	ImageResource::~ImageResource() {
		//Debug("deconstructor of image resource");
	}
	ImageResource* ImageResource::Init(const char*path) {
		ImageResource*ir = nullptr;
		Alice::Data data;
		if (ResourceManager::LoadData(path, data)) {
			ir = new ImageResource;
			Serializer::RuntimeAsset ra;
			if (ra.ParseFromArray(data.mData, data.mDataLen)) {//init image resource from alice asset bundle
				if (!ir->InitWithTextureAssetData((const unsigned char*)ra.rawdata().c_str(), ra.rawdata().size())) {
					delete ir;
					ir = nullptr;
				}
			}
			else {//init image resource from raw image like : png or bmp ...
				if (!ir->InitWithRawImageFileData(data.mData, data.mDataLen)) {
					delete ir;
					ir = nullptr;
				}
			}
		}
		if (ir != nullptr) {
			ir->mName = path;
		}
		return ir;
	}
	bool ImageResource::InitWithRawImageFileData(unsigned char * const data, int dataLen) {
		mBPP = RawImage2Uncompressed32BITRGBA(data, dataLen, mPixelData, mWidth, mHeight);
		return mBPP != 0;
	}
	bool ImageResource::InitWithTextureAssetData(const unsigned char*data, int dataLen) {
		Serializer::TextureInfo ti;
		if (ti.ParseFromArray(data, dataLen)) {
			return InitWithRawImageFileData((unsigned char*)ti.imagedata().c_str(), ti.imagedata().size());
		}
		return false;
	}
	void ImageResource::Save(const char*filePath) {
		if (mBPP == 32){
			SaveRGBAToPNG32(filePath, mPixelData.mData, mWidth, mHeight);
		}
		else if (mBPP == 24){
			SaveRGBToJPEG24(filePath, mPixelData.mData, mWidth, mHeight);
		}else if (mBPP==8){
			SaveRToJPEG8(filePath, mPixelData.mData, mWidth, mHeight);
		}
	}
}
