#include "VBO.h"
void VBO::SetSize(int vertexCount,GLenum hint) {
	mVertexCount = vertexCount;
	CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData)*vertexCount, hint, nullptr);
}
void VBO::SubmitData(void*data, GLsizeiptr size) {
	UpdateData(GL_ARRAY_BUFFER, size, data);
}