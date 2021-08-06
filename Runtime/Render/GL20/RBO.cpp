#include "RBO.h"
#include "Runtime/Debugger/Debugger.h"
namespace Alice{
	RBO::RBO(){
		OGL_CALL(glGenRenderbuffers(1,&mRenderBufferObject));
	}

	RBO::~RBO()
	{
		OGL_CALL(glDeleteRenderbuffers(1,&mRenderBufferObject));
	}

	void RBO::Init(int width, int height) {
		OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferObject));
#ifdef ALICE_IPHONE
		OGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height));
#elif ALICE_ANDROID
        OGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height));
#else
		OGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
#endif
		OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}
	void RBO::Resize(int width, int height) {
		OGL_CALL(glDeleteRenderbuffers(1, &mRenderBufferObject));
		Init(width, height);
	}
}
