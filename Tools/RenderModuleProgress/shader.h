#pragma once
#include "AliceGL.h"
#include "Attribute.h"
#include "UniformUpdater.h"
#include "MaterialProperty.h"

class Shader {
public:
	GLuint mProgram;
	Attribute *mAttributes;
	UniformUpdater *mUniforms;
	std::unordered_map<std::string, MaterialProperty*> mProperties;
	static std::unordered_map<std::string, Shader*> mCachedShaders;
	static Shader* LoadShader(const char *name, const char*vs, const char*fs);
	void Init(const char*vs, const char*fs);
protected:
	void InitAttributes();
	void InitUniforms();
	void OnDetectUniformMatrix4(GLint loc, const char* uniform_name);
	void OnDetectUniformVec4(GLint loc, const char* uniform_name);
	void OnDetectUniformSampler2D(GLint loc, const char* uniform_name);
	void AppendUniformUpdater(UniformUpdater*uniform_updater);
};