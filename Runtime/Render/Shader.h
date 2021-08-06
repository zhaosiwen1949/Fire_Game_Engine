#pragma once
#include "AliceGL.h"
#include "Runtime/Render/GL20/GLSL.h"
namespace Alice{
	struct ShaderUniformDefine{
		GLint mLocation;
		GLenum mType;
		int mSize;
	};
	class Shader : public Object{
	public:
		Shader();
		virtual ~Shader();
		GLSL*mGPUProgram;
		std::unordered_map<std::string, ShaderUniformDefine*> mActiveUniforms;
		ShaderUniformDefine * GetActiveUniform(const char *name);
		void AutoActiveUniform();
	public:
		static std::unordered_map<std::string, Shader*> mCachedShader;
		static Shader * GetCached(const char * name);
		static Shader * CreateStandard(const char * name, const char * vscode, const char * fscode);
	public:
		DECLEAR_ALICE_CLASS(Shader)
	};
}