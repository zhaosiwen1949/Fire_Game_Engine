#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/IO/AliceResource.h"
#include "Runtime/IO/TextureResource.h"
#include "TextureUnit.h"

namespace Alice{
	class TextureCube : public TextureUnit {
	public:
		TextureCube();
		~TextureCube();
		GLenum mMinFilter;
		GLenum mMagFilter;
		GLenum mFilterT;
		GLenum mFilterS;
		GLenum mInternalFormat;
		GLenum mSrcFormat;
		GLuint mWidth, mHeight;
		SmartPtr<Resource> mLeftRes,mRightRes,mFrontRes,mBackRes,mTopRes,mBottomRes;
		static TextureCube*LoadTextureCube(const char*name);
		void InitTextureCube();
		static std::unordered_map<std::string, TextureCube*> mTextureCubes;
	public:
		DECLEAR_ALICE_CLASS(TextureCube)
	};
}