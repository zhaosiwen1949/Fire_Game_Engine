#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/MaterialProperty.h"
#include <unordered_map>
#include <string>
#include "ShaderProperties.h"
#include "Runtime/Serializer/GLSL.serializer.h"
#include "Runtime/Render/Attribute.h"
#include "Runtime/Render/RenderMisc.h"
namespace Alice{
	class Data;
	class GLSL:public Object{
	public:
		GLSL();
		~GLSL();
		static GLuint	CompileShader(GLenum type, const char*shaderSrc);
		static GLuint	CreateProgram(GLuint vs, GLuint fs,GLuint gs=0,GLuint tes=0,GLuint tcs=0);
		void	InitWithSerializedData(Serializer::GLSL*runtimeShader);
		void	MakeAsCurrentShader();
		GLint	GetAttributeLocation(const char*name);
		GLint	GetUniformLocation(const char*name);
		void	DefineAttribute(const char*name,int componentCount,GLenum componentType,bool normalized,int stride,int offset);
	public:
		GLuint	mProgram;
		Attribute*mAttributeHead;
		//gles 2.0里从vertex到fragment的数据带宽是32个32位的float数据，即varing变量加起来小于带宽即可
		static GLSL* LoadGLSL(const char*name);
		static void UnloadGLSL(const char*name);
		static GLSL* GetCachedProgram(const char*glsl); 
		static std::unordered_map<std::string, GLSL*> mCachedGLSL;
		static std::unordered_map<std::string, GLSL*> mBuiltinGLSL;
		static GLSL* CreateStandardGLSL(const char*vscode,const char * fscode);
#if ALICE_EDITOR
		void				Dump();
#endif
	};
}