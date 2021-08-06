#include "FBO.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Render/RenderState.h"
namespace Alice{
	FBO::FBO(GLint fbo):mFrameBufferObject(fbo),mWidth(512),mHeight(512),mRBO(nullptr){
		mPrevFrameBufferObject = 0;
		mClearColor.Set(0.1f, 0.4f, 0.6f, 1.0f);
		if (fbo == -1)
			mbSystemFBO = false;
		else
			mbSystemFBO = true;
	}
	FBO::~FBO(){
		if (nullptr!=mRBO){
			delete mRBO;
		}
		if (false==mbSystemFBO&&mFrameBufferObject!=0){
			OGL_CALL(glDeleteFramebuffers(1, &mFrameBufferObject));
		}
	}
	void FBO::Init(int width, int height) {//offscreen camera:color buffer & rbo depth buffer
		mWidth = width;
		mHeight = height;
		mColorBuffer = new Texture2D;
		mColorBuffer->InitTexture2D(width, height);
		mRBO = new RBO;
		mRBO->Init(width, height);
		OGL_CALL(glGenFramebuffers(1, &mFrameBufferObject));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer->mTextureID, 0));
#ifdef ALICE_IPHONE
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#elif ALICE_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#endif
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE) {
			Error("create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FBO::DetachColorBuffer(GLenum attachpoint) {
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachpoint, GL_TEXTURE_2D, 0, 0));
	}
	void FBO::DetachRenderBuffer(GLenum attachpoint) {
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachpoint, GL_RENDERBUFFER, 0));
	}
	void  FBO::DetachDepthStencilRenderBuffer() {
#ifdef ALICE_IPHONE
		DetachRenderBuffer(GL_DEPTH_ATTACHMENT);
		DetachRenderBuffer(GL_STENCIL_ATTACHMENT);
#elif ALICE_ANDROID
		DetachRenderBuffer(GL_DEPTH_ATTACHMENT);
		DetachRenderBuffer(GL_STENCIL_ATTACHMENT);
#else
		DetachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
#endif
	}
	void FBO::Resize(int width, int height) {
		if (mbSystemFBO){
			mWidth = width;
			mHeight = height;
			return;
		}
		if (mFrameBufferObject==0){//not inited yet
			return;
		}
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBufferObject);
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		if (mRBO!=nullptr){
			DetachDepthStencilRenderBuffer();
			delete mRBO;
			mRBO = nullptr;
		}
		if (mColorBuffer.mPtr!=nullptr){
			DetachColorBuffer(GL_COLOR_ATTACHMENT0);
			mColorBuffer.Release();
		}
		if (mDepthComponentBuffer.mPtr != nullptr) {//init default fbo : created via camera:RenderDepth API 
			mDepthComponentBuffer.Release();
			OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthComponentBuffer->mTextureID, 0));
			glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBufferObject);
#ifdef ALICE_IPHONE
			InitDepthFBOES(width, height);
#elif ALICE_ANDROID
			InitDepthFBOES(width, height);
#else
			InitDepthFBO(width, height);
#endif
		}
		else {//init offscreen camera : created via camera:MakeOffScreen API
			glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBufferObject);
			Init(width, height);
		}
	}
	void FBO::InitDepthPartWithDefaultSettings(int width, int height) {//for post rendering : color buffer is not defined,rbo depth buffer
		mWidth = width;
		mHeight = height;
		mRBO = new RBO;
		mRBO->Init(width, height);
		OGL_CALL(glGenFramebuffers(1, &mFrameBufferObject));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
#ifdef ALICE_IPHONE
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#elif ALICE_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::AttachColorBuffer(int nAttachPoint, Texture2D*texture) {
		mWidth = texture->mWidth;
		mHeight = texture->mHeight;
		mColorBuffer = texture;
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+nAttachPoint, GL_TEXTURE_2D, texture->mTextureID, 0));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::InitDepthFBO(int width, int height) {//for shadow rendering,that need to read depth buffer
		mWidth = width;
		mHeight = height;
		mColorBuffer = new Texture2D;
		mColorBuffer->InitTexture2D(width, height);
		mDepthComponentBuffer = new Texture2D;
		OGL_CALL(glGenFramebuffers(1, &mFrameBufferObject));
		mDepthComponentBuffer->InitDepthTexture(width, height);
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer->mTextureID, 0));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthComponentBuffer->mTextureID, 0));
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE) {
			Info("create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::InitDepthFBOES(int width, int height) {//for opengles : rbo depth buffer,color buffer store the depth data
		mWidth = width;
		mHeight = height;
		mRBO = new RBO;
		mRBO->Init(width, height);
		mDepthComponentBuffer = new Texture2D;
		mDepthComponentBuffer->InitTexture2D(width, height);
		OGL_CALL(glGenFramebuffers(1, &mFrameBufferObject));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDepthComponentBuffer->mTextureID, 0));
#ifdef ALICE_IPHONE
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#elif ALICE_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO->mRenderBufferObject));
#endif
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE) {
			Info("create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FBO::Active() {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glViewport(0, 0, mWidth, mHeight);
		glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
		GlobalRenderState::EnableZWrite(true);
		GlobalRenderState::ColorMask(true, true, true, true);
		GlobalRenderState::StencilMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	void FBO::ActiveDepth() {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glViewport(0, 0, mWidth, mHeight);
		glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
		GlobalRenderState::EnableZWrite(true);
		GlobalRenderState::ColorMask(true, true, true, true);
		GlobalRenderState::StencilMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void FBO::Switch() {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	}
	void FBO::Restore() {
		glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBufferObject);
	}
	void FBO::SetClearColor(float r, float g, float b, float a, int index/* =0 */) {
		mClearColor.Set(r, g, b, a);
	}
}
