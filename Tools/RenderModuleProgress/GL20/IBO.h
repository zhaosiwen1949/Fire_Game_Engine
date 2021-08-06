#pragma once
#include "BufferObject.h"
class IBO :public BufferObject {
public:
	void SetSize(int indexCount, GLenum hint = GL_STATIC_DRAW);
};