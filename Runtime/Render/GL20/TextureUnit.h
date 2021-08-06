#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Utils/SmartPtr.h"
namespace Alice{
	class TextureUnit : public Object{
	public:
		GLuint mTextureID;
		GLenum mTextureType;
		TextureUnit();
		virtual ~TextureUnit();
	};
}
