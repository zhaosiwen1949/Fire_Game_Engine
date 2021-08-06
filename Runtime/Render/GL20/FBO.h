#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
#include "Texture2D.h"
#include "RBO.h"
#include "Runtime/Math/Vector4.h"
namespace Alice{
	struct ColorAttachment {
		SmartPtr<Texture2D> mColorBuffer;
		GLenum mAttachmentPoint;
		Color4f mClearColor;
		bool mbClearColorChanged;
		ColorAttachment() {
			mAttachmentPoint = GL_NONE;
			mClearColor.Set(0.1f, 0.4f, 0.6f);
			mbClearColorChanged = true;
		}
	};
	class FBO {
	public:
		int mWidth, mHeight;
        GLint mPrevFrameBufferObject;
		GLuint mFrameBufferObject;
		Color4f mClearColor;
		SmartPtr<Texture2D> mColorBuffer;
		SmartPtr<Texture2D> mDepthComponentBuffer;
		RBO * mRBO;
		bool mbSystemFBO;
		FBO(GLint fbo=-1);
		virtual ~FBO();
		void SetClearColor(float r, float g, float b, float a, int index=0);
		void Init(int width,int height);
		void Resize(int width,int height);
		void InitDepthFBO(int width, int height);
		void InitDepthFBOES(int width, int height);
		void InitDepthPartWithDefaultSettings(int width, int height);
		void AttachColorBuffer(int nAttachPoint, Texture2D*texture);
		void DetachColorBuffer(GLenum attachpoint);
		void DetachRenderBuffer(GLenum attachpoint);
		void DetachDepthStencilRenderBuffer();
		void Active();
		void ActiveDepth();
		void Switch();
		void Restore();
	};
}
