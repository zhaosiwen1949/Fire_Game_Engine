#pragma once
#include "XVulkan.h"
#include "Texture2D.h"
#include "UniformBuffer.h"
class Material {
public:
	XProgram mProgram;
	XFixedPipeline *mFixedPipeline;
	UniformBuffer*mVertexVec4UBO, *mVertexMat4UBO, *mFragVec4UBO, *mFragMat4UBO;
	XTexture mTexture[8];
public:
	Material();
	~Material();
	void Init(const char *vs, const char *fs = nullptr);
	void SetFixedPipeline(XFixedPipeline*p);
	void SetTexture(int binding, XTexture *texture);
	void SetMVP(const glm::mat4&m, const glm::mat4&v, const glm::mat4&p);
	void Finish();
	void SubmitUniformBuffers();
	static std::set<Material*> mMaterials;
	static void CleanUp();
};