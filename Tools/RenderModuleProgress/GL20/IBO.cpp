#include "IBO.h"
void IBO::SetSize(int indexCount, GLenum hint /* = GL_STATIC_DRAW */) {
	CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indexCount, hint, nullptr);
}