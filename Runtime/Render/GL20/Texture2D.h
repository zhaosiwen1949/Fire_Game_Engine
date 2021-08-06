#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/RenderMisc.h"
#include "Runtime/IO/TextureResource.h"
#include "Runtime/Serializer/TextureInfo.serializer.h"
#include "Runtime/Utils/SmartPtr.h"
#include <map>
#include <string>
#include "TextureUnit.h"

namespace Alice{
	class Texture2D : public TextureUnit{
	public:
		Texture2D();
		~Texture2D();
		void InitTexture2D(AliceUInt16 width = 1024, AliceUInt16 height = 1024,
			GLenum internalformat = GL_RGBA,
			GLenum srcFormat = GL_RGBA,
			GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR,
			GLenum filterS = GL_CLAMP_TO_EDGE,
			GLenum filterT = GL_CLAMP_TO_EDGE
		);
		void InitDepthTexture(AliceUInt16 width=1024,AliceUInt16 height=1024);
		GLenum mMinFilter;
		GLenum mMagFilter;
		GLenum mFilterT;
		GLenum mFilterS;
		GLenum mInternalFormat;
		GLenum mSrcFormat;
		GLuint mWidth,mHeight;
		SmartPtr<Resource> mRes;
		std::unordered_map<std::string, Serializer::Sprite*> mSprites;
		void UploadTexture(AliceAny imageData);
		void TexParameteri(GLenum name, GLenum value);
		void UploadETC1RGBTexture(AliceUInt8*imageData, AliceUInt16 paddedWidth, AliceUInt16 paddedHeight, AliceUInt8*alphaChannel=nullptr);
		void UploadSubTexture(AliceUInt16 x,AliceUInt16 y,AliceUInt16 width,AliceUInt16 height,GLenum format,GLenum dataType,AliceUInt8*pData);
		Serializer::Sprite*GetSprite(const char *name);
		void InitSpriteInfo(Serializer::Texture*texture);
	public:
		//input texture name
		static std::unordered_map<std::string, Texture2D*> mTextures;
		static Texture2D*LoadTexture2D(const char*name);
		static Texture2D*CreateTextureFromMemory(const char *data, int dataLen,GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR,
			GLenum filterS = GL_CLAMP_TO_EDGE,
			GLenum filterT = GL_CLAMP_TO_EDGE);
		static Texture2D*CreateTextureWidthFreeImage(TextureResource*jpg);
		static Texture2D*CreateTextureFromETC1RGBFile(TextureResource*etc1rgb);
		static Texture2D*CreateTextureFromPVRTC4RGBAFile(TextureResource*pvrtcgrba);
		static Texture2D*CreateTextureFromRawLuminance8(TextureResource*pvrtcgrba);
		static Texture2D*GetCachedTexture2D(const char*path);
		static Serializer::ImageTile*GetSpriteInfo(const char*atlasName,const char*spriteName);
		static int mTextureMemoryCount;
		static void UnloadTexture(const char*name);
		static Texture2D* mTextureError;
	public:
		DECLEAR_ALICE_CLASS(Texture2D)
	};
}
