#include "Shader.h"
namespace Alice{
	Shader::Shader() :mGPUProgram(nullptr) {

	}
	Shader::~Shader() {
		if (mGPUProgram!=nullptr){
			delete mGPUProgram;
		}
	}
	ShaderUniformDefine * Shader::GetActiveUniform(const char *name) {
		auto iter = mActiveUniforms.find(name);
		if (iter != mActiveUniforms.end()) {
			return iter->second;
		}
		return nullptr;
	}
	void Shader::AutoActiveUniform() {
		GLint active_uniform_count = 0;
		glGetProgramiv(mGPUProgram->mProgram, GL_ACTIVE_UNIFORMS, &active_uniform_count);
		char szTemp[256];
		GLint size;
		GLenum type;
		for (int index = 0; index < active_uniform_count; ++index){
			memset(szTemp, 0, 256);
			glGetActiveUniform(mGPUProgram->mProgram, index, 256, NULL, &size, &type, szTemp);
			ShaderUniformDefine*sud = new ShaderUniformDefine;
			sud->mLocation = mGPUProgram->GetUniformLocation(szTemp);
			sud->mType = type;
			sud->mSize = size;
			mActiveUniforms.insert(std::pair<std::string,ShaderUniformDefine*>(szTemp,sud));
		}
	}
	std::unordered_map<std::string, Shader*> Shader::mCachedShader;
	Shader*Shader::GetCached(const char * name) {
		auto iter = mCachedShader.find(name);
		if (iter != mCachedShader.end()) {
			return iter->second;
		}
		return nullptr;
	}
	Shader*Shader::CreateStandard(const char * name, const char * vscode, const char * fscode) {
		GLSL*glsl = GLSL::CreateStandardGLSL(vscode, fscode);
		if (glsl == nullptr) {
			Error("Shader::CreateStandard failed %s", name);
			return nullptr;
		}
		glsl->mName = name;
		Shader*shader = new Shader;
		shader->mName = name;
		shader->mGPUProgram = glsl;
		shader->AutoActiveUniform();
		mCachedShader.insert(std::pair<std::string, Shader*>(name, shader));
		return shader;
	}
}
