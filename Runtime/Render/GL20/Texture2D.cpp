#include "Texture2D.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "Runtime/Plugins/freeimage/FreeImage.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Serializer/TextureAtlas.serializer.h"
#include "Runtime/Render/RenderState.h"
#include "Runtime/Debugger/Debugger.h"
namespace Alice{
	Texture2D* Texture2D::mTextureError;
	std::unordered_map<std::string, Texture2D*> Texture2D::mTextures;
	static std::map<std::string, Serializer::TextureAtlas*> sCachedAtlas;
	int Texture2D::mTextureMemoryCount = 0;
	Texture2D::Texture2D():mRes(nullptr) {
		mName = "Texture2D";
		mTextureType = GL_TEXTURE_2D;
		mWidth = 0;
		mHeight = 0;
		mTextureID = 0;
	}

	Texture2D::~Texture2D(){
		std::unordered_map<std::string, Texture2D*>::iterator iter = mTextures.find(mName.mText);
		if (iter != mTextures.end()) {
			mTextures.erase(iter);
		}
	}

	void Texture2D::InitTexture2D(AliceUInt16 width /* = 1024 */, AliceUInt16 height /* = 1024 */, 
		GLenum internalformat /* = GL_RGBA */, GLenum srcFormat /* = GL_RGBA */, 
		GLenum minFilter /* = GL_LINEAR */, GLenum magFilter /* = GL_LINEAR */, 
		GLenum filterS /* = GL_CLAMP_TO_EDGE */, GLenum filterT /* = GL_CLAMP GL_CLAMP_TO_EDGE */){
		if (mTextureID!=0){
			OGL_CALL(glDeleteTextures(1, &mTextureID));
			mTextureID = 0;
		}
		mWidth = width;
		mHeight = height;
		mInternalFormat = internalformat;
		mSrcFormat = srcFormat;
		mMinFilter = minFilter;
		mMagFilter = magFilter;
		mFilterT = filterT;
		mFilterS = filterS;
		OGL_CALL(glGenTextures(1, &mTextureID));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mFilterS));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mFilterT));
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mSrcFormat, GL_UNSIGNED_BYTE, nullptr));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::InitDepthTexture(AliceUInt16 width/* =1024 */, AliceUInt16 height/* =1024 */) {
		mWidth = width;
		mHeight = height;
		mInternalFormat = GL_DEPTH_COMPONENT;
		mSrcFormat = GL_DEPTH_COMPONENT;
		mMinFilter = GL_LINEAR;
		mMagFilter = GL_LINEAR;
		mFilterT = GL_CLAMP_TO_EDGE;
		mFilterS = GL_CLAMP_TO_EDGE;
		OGL_CALL(glGenTextures(1, &mTextureID));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mFilterS));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mFilterT));
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mSrcFormat, GL_FLOAT, nullptr));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::TexParameteri(GLenum name, GLenum value){
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, name, value));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::UploadTexture(AliceAny imageData){
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D,0, mInternalFormat, (GLsizei)mWidth, (GLsizei)mHeight, 0, mSrcFormat,GL_UNSIGNED_BYTE, imageData));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::UploadETC1RGBTexture(AliceUInt8*imageData,AliceUInt16 paddedWidth, AliceUInt16 paddedHeight,AliceUInt8*alphaChannel){
#if ALICE_ANDROID
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		OGL_CALL(glCompressedTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, (GLsizei)mWidth, (GLsizei)mHeight, 0, (paddedWidth * paddedHeight) >> 1, imageData + 16));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
		/*if (alphaChannel!=nullptr)
		{
			GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID1);
			OGL_CALL(glCompressedTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, (GLsizei)mWidth, (GLsizei)mHeight, 0, (paddedWidth * paddedHeight) >> 1, alphaChannel + 16));
			GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
		}*/
#endif
	}
	void Texture2D::UploadSubTexture(AliceUInt16 x, AliceUInt16 y, 
		AliceUInt16 width, AliceUInt16 height, 
		GLenum format, GLenum dataType, AliceUInt8*pData){
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, mTextureID);
		//Info("upload sub texture %d \n\texture top left %d %d\n\texture width %d height:%d",mTextureID,x,y,width,height);
		OGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, dataType, pData));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::UnloadTexture(const char*name){
		if (name == nullptr){
			return;
		}
		Texture2D*texture = nullptr;
		std::unordered_map<std::string, Texture2D*>::iterator iter = mTextures.find(name);
		if (iter != mTextures.end()){
			texture = iter->second;
			mTextures.erase(iter);
			Resource::Unload(name);
		}
	}
	Serializer::Sprite*Texture2D::GetSprite(const char *name) {
		auto iter = mSprites.find(name);
		if (iter != mSprites.end()) {
			return iter->second;
		}
		return nullptr;
	}
	void Texture2D::InitSpriteInfo(Serializer::Texture*texture) {
		int count = texture->sprites_size();
		for (int i=0;i<count;++i){
			Serializer::Sprite*sprite = new Serializer::Sprite;
			*sprite = texture->sprites(i);
			mSprites.insert(std::pair<std::string,Serializer::Sprite*>(sprite->name(),sprite));
		}
	}
	Serializer::ImageTile*Texture2D::GetSpriteInfo(const char*atlasName, const char*spriteName){
		std::map<std::string, Serializer::TextureAtlas*>::iterator iter = sCachedAtlas.find(atlasName);
		if (iter != sCachedAtlas.end()){
			int nCount = iter->second->imagetiles_size();
			for (int i = 0; i < nCount; i++){
				if (strcmp(spriteName,iter->second->imagetiles(i).name().c_str())==0){
					return iter->second->mutable_imagetiles(i);
				}
			}
		}
		return nullptr;
	}
	Texture2D*Texture2D::GetCachedTexture2D(const char*path){
		if (path==nullptr){
			return nullptr;
		}
		auto iter = mTextures.find(path);
		if (iter !=mTextures.end()){
			return iter->second;
		}
		return nullptr;
	}
	Texture2D* Texture2D::CreateTextureFromMemory(const char *image_data, int image_data_len,
		GLenum minFilter, GLenum magFilter, GLenum filterS, GLenum filterT){
		Data data;
		GLenum cpuFormat = GL_RGBA;
		GLenum gpuFormat = GL_RGBA;
		int nWidth = 0, nHeight = 0;
		int bpp = RawImage2Uncompressed32BITRGBA((AliceUInt8*)image_data, image_data_len, data, nWidth, nHeight);
		switch (bpp)
		{
		case 32:
			break;
		case 24:
			cpuFormat = GL_RGB;
			gpuFormat = GL_RGB;
			break;
		case 8:
			cpuFormat = GL_ALPHA;
			gpuFormat = GL_ALPHA;
			break;
		default:
			errorC("file bpp is not supported,must be 8 or 24 or 32");
			return nullptr;
		}
		Texture2D *texture2D = new Texture2D;
		texture2D->InitTexture2D(nWidth, nHeight, gpuFormat, cpuFormat,minFilter,magFilter,filterS,filterT);
		texture2D->UploadTexture(data.mData);
		return texture2D;
	}
	Texture2D* Texture2D::CreateTextureWidthFreeImage(TextureResource*texture_resource){
		Texture2D*ret = nullptr;
		switch (texture_resource->mTextureResourceType){
		case kTextureResourceTypeTexture:{
			Serializer::Texture*texture = texture_resource->RuntimeData<Serializer::Texture>();
			ret = CreateTextureFromMemory(texture->image_data().c_str(), texture->image_data().length(),
				texture->min_filter(),texture->mag_filter(),texture->wrap_mode_u(),texture->wrap_mode_v());
			ret->InitSpriteInfo(texture);
		}
			break;
		case kTextureResourceTypeTextureInfo:{
			Serializer::TextureInfo*ti = texture_resource->RuntimeData<Serializer::TextureInfo>();
			ret = CreateTextureFromMemory(ti->imagedata().c_str(), ti->imagedata().length());
		}
			break;
		}
		return ret;
	}
	Texture2D*Texture2D::CreateTextureFromPVRTC4RGBAFile(TextureResource*pvrtcgrba){
		return mTextureError;
	}
	Texture2D*Texture2D::CreateTextureFromRawLuminance8(TextureResource*rawLuminance8){
		Serializer::TextureInfo*ti = rawLuminance8->RuntimeData<Serializer::TextureInfo>();
		Debug("CreateTextureFromRawLuminance8 %dx%d", ti->width(), ti->height());
		Texture2D *texture2D = new Texture2D;
#if !ALICE_OSX_PLAYER
		texture2D->InitTexture2D(ti->width(), ti->height(), GL_LUMINANCE, GL_LUMINANCE);
		texture2D->UploadTexture((AliceUInt8*)ti->imagedata().c_str());
#endif
		return texture2D;
	}
	Texture2D*Texture2D::CreateTextureFromETC1RGBFile(TextureResource*etc1rgb){
#if ALICE_ANDROID
		Serializer::ETC1Data etc1Data;
		Serializer::TextureInfo*ti = etc1rgb->RuntimeData<Serializer::TextureInfo>();
		if (etc1Data.ParseFromArray(ti->imagedata().c_str(), ti->imagedata().length()))
		{
			AliceUInt8 *imageData = (AliceUInt8*)etc1Data.mutable_rgb()->c_str();
			AliceUInt16 nWidth = (imageData[12] << 8) | imageData[13];
			AliceUInt16 nHeight = (imageData[14] << 8) | imageData[15];
			AliceUInt16 paddedWidth = (imageData[8] << 8) | imageData[9];
			AliceUInt16 paddedHeight = (imageData[10] << 8) | imageData[11];

			Texture2D *texture2D = new Texture2D;
			texture2D->InitTexture2D(nWidth, nHeight, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_ETC1_RGB8_OES, GL_RGB);
			AliceUInt8 *alphaData = nullptr;
			if (etc1Data.has_alpha()){
				/*alphaData = (AliceUInt8*)etc1Data.mutable_alpha()->c_str();
				OGL_CALL(glGenTextures(1, &texture2D->mTextureID1));
				GlobalRenderState::BindTexture(GL_TEXTURE_2D, texture2D->mTextureID1);
				OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture2D->mMinFilter));
				OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture2D->mMagFilter));
				OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture2D->mFilterS));
				OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture2D->mFilterT));
				OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, texture2D->mInternalFormat, texture2D->mWidth, texture2D->mHeight, 0, texture2D->mSrcFormat, GL_UNSIGNED_BYTE, NULL));
				GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);*/
			}
			texture2D->UploadETC1RGBTexture(imageData, paddedWidth, paddedHeight, alphaData);
			return texture2D;
		}
#endif
		return mTextureError;
	}
	Texture2D* Texture2D::LoadTexture2D(const char*name){
		if (name ==nullptr || strlen(name)==0){
			return mTextureError;
		}
		std::unordered_map<std::string, Texture2D*>::iterator iter = mTextures.find(name);
		if (iter != mTextures.end()) {
			return iter->second;
		}
		TextureResource*textureResource = Resource::LoadResource<TextureResource>(name);
		if (textureResource==nullptr){
			Error("Texture2D::LoadTexture2D load %s failed", name);
			return mTextureError;
		}
		Texture2D*texture = nullptr;
		switch (textureResource->mType){
		case ResourceTypePNGImage:
		case ResourceTypeBMPImage:
		case ResourceTypeTGAImage:
		case ResourceTypeJPGImage:
			texture = CreateTextureWidthFreeImage(textureResource);
			break;
		case ResourceTypeETC1RGB:
			texture = CreateTextureFromETC1RGBFile(textureResource);
			break;
		case ResourceTypePVRTCRGBA:
			texture = CreateTextureFromPVRTC4RGBAFile(textureResource);
			break;
		case ResourceTypeRawImageLUMINANCE:
			texture = CreateTextureFromRawLuminance8(textureResource);
			break;
		}
		delete textureResource;
		if (texture == nullptr) {
			Error("Texture2D::LoadTexture2D decode %s failed", name);
			return mTextureError;
		}
		texture->mName = name;
		mTextures.insert(std::pair<std::string, Texture2D*>(name, texture));
		return texture;
	}
}
