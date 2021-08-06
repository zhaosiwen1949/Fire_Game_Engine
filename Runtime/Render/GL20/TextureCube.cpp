#include "TextureCube.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Serializer/TextureInfo.serializer.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "Runtime/Render/RenderState.h"

#pragma warning(disable:4700)
namespace Alice{
	std::unordered_map<std::string, TextureCube*> TextureCube::mTextureCubes;
	TextureCube::TextureCube(){
		OGL_CALL(glGenTextures(1, &mTextureID));
		mTextureType = GL_TEXTURE_CUBE_MAP;
	}
	TextureCube::~TextureCube() {
		std::unordered_map<std::string, TextureCube*>::iterator iter = mTextureCubes.find(mName.mText);
		if (iter != mTextureCubes.end()) {
			mTextureCubes.erase(iter);
		}
	}
	void TextureCube::InitTextureCube() {
		GlobalRenderState::BindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		Data pixelData;
		int width = 0, height = 0;
		Serializer::TextureInfo*ti = mFrontRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		//front
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		//back
		pixelData.Reset();
		width = 0, height = 0;
		ti = mBackRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		//left
		pixelData.Reset();
		width = 0, height = 0;
		ti = mLeftRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		//right
		pixelData.Reset();
		width = 0, height = 0;
		ti = mRightRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		//top
		pixelData.Reset();
		width = 0, height = 0;
		ti = mTopRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		//bottom
		pixelData.Reset();
		width = 0, height = 0;
		ti = mBottomRes->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)ti->imagedata().c_str(), ti->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.mData));
		GlobalRenderState::BindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	TextureCube* TextureCube::LoadTextureCube(const char*folderpath) {
		if (folderpath == nullptr || strlen(folderpath) == 0) {
			return nullptr;
		}
		std::unordered_map<std::string, TextureCube*>::iterator iter = mTextureCubes.find(folderpath);
		if (iter != mTextureCubes.end()) {
			return iter->second;
		}
		char temp[256];
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/front.jpg");
		TextureResource*frontRes = Resource::LoadResource<TextureResource>(temp);
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/back.jpg");
		TextureResource*backRes = Resource::LoadResource<TextureResource>(temp);
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/left.jpg");
		TextureResource*leftRes = Resource::LoadResource<TextureResource>(temp);
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/right.jpg");
		TextureResource*rightRes = Resource::LoadResource<TextureResource>(temp);
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/top.jpg");
		TextureResource*topRes = Resource::LoadResource<TextureResource>(temp);
		memset(temp, 0, 256);
		strcpy(temp, folderpath);
		strcat(temp, "/bottom.jpg");
		TextureResource*bottomRes = Resource::LoadResource<TextureResource>(temp);
		TextureCube*textureCube = new TextureCube;
		textureCube->mFrontRes = backRes;
		textureCube->mBackRes = frontRes;
		textureCube->mLeftRes = leftRes;
		textureCube->mRightRes = rightRes;
		textureCube->mTopRes = bottomRes;
		textureCube->mBottomRes = topRes;
		textureCube->mName = folderpath;
		textureCube->InitTextureCube();
		mTextureCubes.insert(std::pair<std::string, TextureCube*>(folderpath, textureCube));
		return textureCube;
	}
}
