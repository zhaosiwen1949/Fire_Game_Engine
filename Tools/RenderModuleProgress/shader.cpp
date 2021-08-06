#include "shader.h"
#include "utils.h"
#include "VertexData.h"
std::unordered_map<std::string,Shader*> Shader::mCachedShaders;
Shader*Shader::LoadShader(const char *name, const char*vs, const char*fs) {
	auto iter = mCachedShaders.find(name);
	if (iter!=mCachedShaders.end()){
		return iter->second;
	}
	Shader*shader = new Shader;
	shader->Init(vs, fs);
	mCachedShaders.insert(std::pair<std::string,Shader*>(name,shader));
	return shader;
}
void Shader::Init(const char*vs, const char*fs) {
	mAttributes = nullptr;
	mUniforms = nullptr;
	int nFileSize = 0;
	const char*vsCode = (char*)LoadFileContent(vs, nFileSize);
	const char*fsCode = (char*)LoadFileContent(fs, nFileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
	if (vsShader == 0) {
		return;
	}
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if (fsShader == 0) {
		return;
	}
	mProgram = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	if (mProgram != 0) {
		InitAttributes();
		InitUniforms();
	}
}
void Shader::InitAttributes() {
	GLint count = 0;
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &count);
	for (int i = 0; i < count; ++i) {
		GLsizei len = 0;
		GLenum t = 0;
		GLint s = 0;
		char attribute_name[256] = { 0 };
		glGetActiveAttrib(mProgram, i, 256, &len, &s, &t, attribute_name);
		GLint loc = glGetAttribLocation(mProgram, attribute_name);
		int component_count = 0;
		int base_data_type = 0;
		if (t == GL_FLOAT_VEC4) {
			component_count = 4;
			base_data_type = GL_FLOAT;
		}
		int data_offset = 0;
		if (strcmp(attribute_name, "position") == 0) {
			data_offset = 0;
		}
		else if (strcmp(attribute_name, "texcoord") == 0) {
			data_offset = sizeof(float) * 4;
		}
		else if (strcmp(attribute_name, "normal") == 0) {
			data_offset = sizeof(float) * 8;
		}
		else if (strcmp(attribute_name, "tangent") == 0) {
			data_offset = sizeof(float) * 12;
		}
		else if (strcmp(attribute_name, "texcoord1") == 0) {
			data_offset = sizeof(float) * 16;
		}
		Attribute*attribute = new Attribute(loc,
			component_count,
			base_data_type,
			GL_FALSE,
			sizeof(VertexData),
			data_offset);
		if (mAttributes == nullptr) {
			mAttributes = attribute;
		}
		else {
			mAttributes->PushBack(attribute);
		}
	}
}
void Shader::InitUniforms() {
	GLint count = 0;
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; ++i) {
		GLsizei len = 0;
		GLenum t = 0;
		GLint s = 0;
		char uniform_name[256] = { 0 };
		glGetActiveUniform(mProgram, i, 256, &len, &s, &t, uniform_name);
		GLint loc = glGetUniformLocation(mProgram, uniform_name);
		if (t == GL_FLOAT_MAT4) {
			OnDetectUniformMatrix4(loc, uniform_name);
		}
		else if (t == GL_FLOAT_VEC4) {
			OnDetectUniformVec4(loc, uniform_name);
		}
		else if (t == GL_SAMPLER_2D) {
			OnDetectUniformSampler2D(loc, uniform_name);
		}
	}
}
void Shader::OnDetectUniformMatrix4(GLint loc, const char* uniform_name) {
	MaterialPropertyMatrix4*prop = nullptr;
	MaterialUniformPropertyType uniform_type = kMaterialUniformPropertyTypeMatrix4;
	UpdateUniformFunction  update_function = UniformUpdater::UpdateMatrix4;
	if (strcmp(uniform_name, "Alice_ProjectionMatrix") == 0) {
		uniform_type = kMaterialUniformPropertyTypeCameraProjectionMatrix;
		update_function = UniformUpdater::UpdateProjectionMatrix4;
	}
	else if (strcmp(uniform_name, "Alice_ViewMatrix") == 0) {
		uniform_type = kMaterialUniformPropertyTypeCameraViewMatrix;
		update_function = UniformUpdater::UpdateViewMatrix4;
	}
	else {
		prop = new MaterialPropertyMatrix4(uniform_name);
		prop->mType = uniform_type;
		mProperties.insert(std::pair<std::string, MaterialProperty*>(uniform_name, prop));
	}
	AppendUniformUpdater(new UniformUpdater(loc, update_function, prop));
}
void Shader::OnDetectUniformVec4(GLint loc, const char * uniform_name) {
	MaterialPropertyVec4*prop = new MaterialPropertyVec4(uniform_name);
	prop->mType = kMaterialUniformPropertyTypeVec4;
	mProperties.insert(std::pair<std::string, MaterialProperty*>(uniform_name, prop));
	AppendUniformUpdater(new UniformUpdater(loc, UniformUpdater::UpdateVec4, prop));
}
void Shader::OnDetectUniformSampler2D(GLint loc, const char* uniform_name) {
	MaterialPropertySampler2D* prop = new MaterialPropertySampler2D(uniform_name);
	prop->mType = kMaterialUniformPropertyTypeSampler2D;
	mProperties.insert(std::pair<std::string, MaterialProperty*>(uniform_name, prop));
	AppendUniformUpdater(new UniformUpdater(loc, UniformUpdater::UpdateSampler2D, prop));
}
void Shader::AppendUniformUpdater(UniformUpdater*uniform_updater) {
	if (mUniforms == nullptr) {
		mUniforms = uniform_updater;
	}
	else {
		mUniforms->PushBack(uniform_updater);
	}
}