#pragma once
#include "BufferObject.h"

namespace Alice{
	class EBO :public BufferObject{
	public:
		EBO(int nCount=0, GLenum hint = GL_DYNAMIC_DRAW);
		virtual ~EBO();
		AliceUInt16 mIndexCount;
		AliceUInt16 mIndexCountForDraw;
		AliceUInt16 mStartIndex;
		void Init(int indexcount, GLenum hint=GL_DYNAMIC_DRAW);
		void Draw(GLenum primitiveType);
	};
}
