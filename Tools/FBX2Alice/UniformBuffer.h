#pragma once
#include "XVulkan.h"
class UniformBuffer : public XBufferObject {
public:
	std::vector<XMatrix4x4f> mMatrices;
	std::vector<XVector4f> mVector4s;
public:
	XUniformBufferType mUniformBufferType;
	UniformBuffer(XUniformBufferType t);
	~UniformBuffer();
	void SetSize(int count);
	int GetSize();
	void SubmitData();
	void SetMatrix(int location, const float *v);
	void SetMatrix(int location, const glm::mat4 &m);
	void SetVector4(int location, const float *v);
	void SetVector4(int location, float x,float y,float z,float w);
};