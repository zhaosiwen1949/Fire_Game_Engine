#pragma once
#include "BufferObject.h"
namespace Alice{
	class VBO:public BufferObject{
	public:
		VBO(AliceUInt16 nSize=0,GLenum hint=GL_DYNAMIC_DRAW);
		virtual ~VBO();
		AliceUInt16 mSize;
		AliceUInt16 mDrawStartIndex;
		AliceUInt16 mDrawCount;
		void Init(int count, GLenum hint = GL_DYNAMIC_DRAW);
		void Draw(GLenum primitiveType);
	};
}