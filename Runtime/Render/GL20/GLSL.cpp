#include "GLSL.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/ShaderResource.h"
#include "Runtime/IO/ResourceManager.h"
#include <stdlib.h>
#include <sstream>
namespace	Alice{
	std::unordered_map<std::string, GLSL*> GLSL::mCachedGLSL;
	std::unordered_map<std::string, GLSL*> GLSL::mBuiltinGLSL;
	GLSL*GLSL::GetCachedProgram(const char*glsl){
		GLSL*ret = nullptr;
		if (mCachedGLSL.find(glsl) != mCachedGLSL.end()){
			ret = mCachedGLSL[glsl];
		}
		return ret;
	}
	void GLSL::InitWithSerializedData(Serializer::GLSL*runtimeShader){
		GLuint vsShader, fsShader;
		vsShader = CompileShader(GL_VERTEX_SHADER, runtimeShader->vert_code().c_str());
		fsShader = CompileShader(GL_FRAGMENT_SHADER, runtimeShader->frag_code().c_str());
		mProgram = CreateProgram(vsShader, fsShader);
		OGL_CALL(glDeleteShader(vsShader));
		OGL_CALL(glDeleteShader(fsShader));
        if (mProgram==0) {
            Error("create program fail : %s\n%s\n",runtimeShader->vert_code().c_str(),runtimeShader->frag_code().c_str());
            return ;
        }
		int nCount = runtimeShader->attributes_size();
		for (int i = 0; i < nCount; i++)
		{
			const Serializer::AttributeDefine&attribute = runtimeShader->attributes(i);
			
			GLint attriPos = glGetAttribLocation(mProgram, attribute.name().c_str());
			if (attriPos!=-1)
			{
				DefineAttribute(attribute.name().c_str(), attribute.component_count(), (GLenum)attribute.data_type(), attribute.normalized() == 1 ? GL_TRUE : GL_FALSE, attribute.stride(), attribute.offset());
			}
			else
			{
				Error("unoptimized attribute %s in shader",attribute.name().c_str());
			}
		}
		if (nCount == 0) {
			DefineAttribute("pos", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), 0);
			DefineAttribute("texcoord", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 4);
			DefineAttribute("normal", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 8);
			DefineAttribute("tangent", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 12);
			DefineAttribute("texcoord1", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 16);
		}
	}

	void GLSL::UnloadGLSL(const char*name){
		Resource::Unload(name);
		std::unordered_map<std::string, GLSL*>::iterator iter = mCachedGLSL.find(name);
		if (iter!=mCachedGLSL.end()){
			delete iter->second;
			mCachedGLSL.erase(iter);
		}
	}
	GLSL*GLSL::CreateStandardGLSL(const char*vscode, const char * fscode) {
		GLint vs = CompileShader(GL_VERTEX_SHADER, vscode);
		if (vs == 0) {
			return nullptr;
		}
		GLint fs = CompileShader(GL_FRAGMENT_SHADER, fscode);
		if (fs == 0) {
			OGL_CALL(glDeleteShader(vs));
			return nullptr;
		}
		GLuint program = CreateProgram(vs, fs);
		if (vs != 0)OGL_CALL(glDeleteShader(vs));
		if (fs != 0)OGL_CALL(glDeleteShader(fs));
		if (program == 0) {
			return nullptr;
		}
		GLSL*glsl = new (kMemDefaultId)GLSL;
		glsl->mProgram = program;
		glsl->DefineAttribute("pos", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), 0);
		glsl->DefineAttribute("texcoord", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 4);
		glsl->DefineAttribute("normal", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 8);
		glsl->DefineAttribute("tangent", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 12);
		glsl->DefineAttribute("texcoord1", 4, GL_FLOAT, GL_FALSE, sizeof(Alice::VertexDataFull), sizeof(float) * 16);
		return glsl;
	}
	GLSL*GLSL::LoadGLSL(const char*name)
	{
		GLSL*glsl = GetCachedProgram(name);
		if (glsl==nullptr)
		{
			ShaderResource*shaderResource = Resource::LoadResource<ShaderResource>(name);
			if (shaderResource!=nullptr)
			{
				Serializer::GLSL*runtimeShader = shaderResource->RuntimeData<Serializer::GLSL>();
				glsl = new (kMemGfxDeviceId)GLSL;
				glsl->InitWithSerializedData(runtimeShader);
				if (glsl->mProgram == 0)
				{
					delete glsl;
					Error("fatal GLSL::CreateGLSLWithSerializedData %s", name);
					return nullptr;
				}
				glsl->mName = name;
				mCachedGLSL.insert(std::pair<std::string, GLSL*>(name, glsl));
				if (Alice::StringUtils::StartWith(name, "builtin"))
				{
					mBuiltinGLSL.insert(std::pair<std::string, GLSL*>(name, glsl));
				}
			}
			else
			{
				Error("cannot load shader %s",name);
			}
		}
		return glsl;
	}

	GLSL::GLSL():Object(1),mAttributeHead(nullptr),mProgram(0){
	}

#if ALICE_EDITOR
	void GLSL::Dump()
	{
	}
#endif
	GLuint GLSL::CompileShader(GLenum type, const char*shaderSrc){
		GLuint ret = 0;
		OGL_CALL(ret = glCreateShader(type));
		if (0 == ret){
			return 0;
		}
		OGL_CALL(glShaderSource(ret, 1, &shaderSrc, NULL));
		OGL_CALL(glCompileShader(ret));
		GLint nResult;
		OGL_CALL(glGetShaderiv(ret, GL_COMPILE_STATUS, &nResult));
		if (GL_FALSE == nResult){
			char log[1024] = {0};
			GLsizei written;
			OGL_CALL(glGetShaderInfoLog(ret, 1024, &written, log));
			Error("shader compile error : %s", log);
			Error("shader code %s", shaderSrc);
			OGL_CALL(glDeleteShader(ret));
			return 0;
		}
		return ret;
	}

	GLuint GLSL::CreateProgram(GLuint vs, GLuint fs, GLuint gs, GLuint tes, GLuint tcs)
	{
		GLuint program=0;
		GLint nResult=0;
		OGL_CALL(program = glCreateProgram());
		if (0 == program){
			Error("cannot create gpu program");
			return program;
		}
		OGL_CALL(glAttachShader(program, vs));
		OGL_CALL(glAttachShader(program, fs));
		if (gs!=0){
			OGL_CALL(glAttachShader(program, gs));
		}
		if (tes!=0){
			OGL_CALL(glAttachShader(program, tes));
		}
		if (tcs!=0){
			OGL_CALL(glAttachShader(program, tcs));
		}

		OGL_CALL(glLinkProgram(program));
		OGL_CALL(glDetachShader(program, vs));
		OGL_CALL(glDetachShader(program, fs));

		if (gs != 0){
			OGL_CALL(glDetachShader(program, gs));
		}
		if (tes != 0){
			OGL_CALL(glDetachShader(program, tes));
		}
		if (tcs != 0){
			OGL_CALL(glDetachShader(program, tcs));
		}
		OGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &nResult));
		if (GL_FALSE == nResult){
			char log[1024] = { 0 };
			GLsizei written;
			OGL_CALL(glGetProgramInfoLog(program, 1024, &written, log));
			Error("GLSL Link Error:%s\n", log);
			OGL_CALL(glDeleteProgram(program));
			program = 0;
		}
		return program;
	}

	void	GLSL::MakeAsCurrentShader()
	{
		OGL_CALL(glUseProgram(mProgram));
		if (mAttributeHead!=nullptr)
		{
			mAttributeHead->Enable();
		}
	}

	GLint GLSL::GetAttributeLocation(const char*name)
	{
		if (mProgram!=0)
		{
			return glGetAttribLocation(mProgram,name);
		}
		return -1;
	}

	GLint GLSL::GetUniformLocation(const char*name){
		if (mProgram != 0){
			return glGetUniformLocation(mProgram, name);
		}
		return -1;
	}
	void GLSL::DefineAttribute(const char*name, int componentCount, GLenum componentType, bool normalized, int stride, int offset){
		int nLoc = GetAttributeLocation(name);
		if (nLoc!=-1){
			Attribute*attri = new (kMemShaderId)Attribute;
			attri->mAttributeSlot = GetAttributeLocation(name);
			attri->mDataOffSet = offset;
			attri->mDataType = componentType;
			attri->mNormalized = normalized;
			attri->mUnitCount = componentCount;
			attri->mDataStride = stride;
			if (mAttributeHead == nullptr){
				mAttributeHead = attri;
			}else{
				mAttributeHead->PushBack(attri);
			}
		}
	}

	GLSL::~GLSL(){
		glUseProgram(0);
		glDeleteProgram(mProgram);
		if (mAttributeHead!=nullptr)
		{
			delete mAttributeHead;
		}
	}
}
