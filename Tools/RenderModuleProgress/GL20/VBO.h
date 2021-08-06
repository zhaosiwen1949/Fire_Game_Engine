#pragma once
#include "BufferObject.h"
#include "../VertexData.h"
class VBO :public BufferObject {
public:
	int mVertexCount;
	void SetSize(int vertexCount,GLenum hint=GL_STATIC_DRAW);
	void SubmitData(void*data, GLsizeiptr size);
};