#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
namespace Alice{
	class RBO{
	public:
		GLuint mRenderBufferObject;
		RBO();
		~RBO();
		void Init(int width,int height);
		void Resize(int width, int height);
	};
}