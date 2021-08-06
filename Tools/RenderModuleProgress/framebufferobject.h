#pragma once
#include "AliceGL.h"
class FrameBufferObject{
public:
	GLuint mFrameBufferObject;
	GLint mPrevFrameBuffer;
	std::map<std::string, GLuint> mBuffers;
	std::vector<GLenum> mDrawBuffers;
public:
	FrameBufferObject();
	void AttachColorBuffer(const char*bufferName,GLenum attachment,int width,int height,GLenum format=GL_RGBA);
	void AttachDepthBuffer(const char*bufferName, int width, int height);
	void Finish();
	void Bind();
	void Unbind();
	GLuint GetBuffer(const char*bufferName);
};