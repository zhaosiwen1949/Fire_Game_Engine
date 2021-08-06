#include "VBO.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Render/RenderState.h"
namespace Alice{
	VBO::VBO(AliceUInt16 nSize, GLenum hint):BufferObject(GL_ARRAY_BUFFER,nSize*sizeof(VertexDataFull),hint){
		mSize = nSize;
		mDrawStartIndex = 0;
		mDrawCount = mSize;
	}
	VBO::~VBO() {
        Active();
		GlobalRenderState::EnableAttribute(0, false);
		GlobalRenderState::EnableAttribute(1, false);
		GlobalRenderState::EnableAttribute(2, false);
		GlobalRenderState::EnableAttribute(3, false);
		GlobalRenderState::EnableAttribute(4, false);
		GlobalRenderState::EnableAttribute(5, false);
		GlobalRenderState::EnableAttribute(6, false);
		GlobalRenderState::EnableAttribute(7, false);
        Restore();
	}
	void VBO::Init(int count, GLenum hint) {
		mSize = count;
		mDrawCount = mSize;
		BufferObject::Init(GL_ARRAY_BUFFER, mSize * sizeof(VertexDataFull), hint);
	}
	void VBO::Draw(GLenum primitiveType) {
		glDrawArrays(primitiveType, mDrawStartIndex, mDrawCount);
	}
}
