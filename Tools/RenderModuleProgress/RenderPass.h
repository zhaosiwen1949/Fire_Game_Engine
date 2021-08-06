#pragma once
#include "AliceGL.h"
#include "Attribute.h"
#include "MaterialProperty.h"
#include "UniformUpdater.h"
#include "RenderState.h"
#include "shader.h"
class RenderPass {
public:
	Shader*mShader;
	RenderState mRenderState;
	UniformUpdater *mUniforms;
	std::unordered_map<std::string, MaterialProperty*> mProperties;
	void SetMatrix4(const char *uniform_name, float*mat4);
	void SetVec4(const char *uniform_name, float*ptr);
	void SetSampler2D(const char* uniform_name, GLuint texture_name);
	void SetShader(Shader*shader);
	void Bind(Camera*camera);
	void AppendUniformUpdater(UniformUpdater*uniform_updater);
};