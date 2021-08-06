#include "model.h"
#include "utils.h"
Model::Model() {
}
void Model::Init(const char*modelPath) {
	struct FloatData {
		float v[3];
	};
	struct VertexDefine {
		int posIndex;
		int texcoordIndex;
		int normalIndex;
	};
	int nFileSize = 0;
	unsigned char*fileContent = LoadFileContent(modelPath, nFileSize);
	if (fileContent==nullptr){
		return;
	}
	std::vector<FloatData> positions, texcoords, normals;
	std::vector<VertexDefine> vertexes;
	std::stringstream ssFileContent((char*)fileContent);
	std::string temp;
	char szOneLine[256];
	while (!ssFileContent.eof()){
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine) > 0){
			if (szOneLine[0] == 'v'){
				std::stringstream ssOneLine(szOneLine);
				if (szOneLine[1] == 't') {
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					texcoords.push_back(floatData);
					printf("texcoord : %f,%f\n", floatData.v[0], floatData.v[1]);
				}else if (szOneLine[1] == 'n') {
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					normals.push_back(floatData);
					printf("normal : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				}else {
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					positions.push_back(floatData);
					printf("position : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				}
			}
			else if (szOneLine[0] == 'f') {
				std::stringstream ssOneLine(szOneLine);
				ssOneLine >> temp;
				std::string vertexStr;
				for (int i = 0; i < 3; i++) {
					ssOneLine >> vertexStr;
					size_t pos = vertexStr.find_first_of('/');
					std::string posIndexStr = vertexStr.substr(0, pos);
					size_t pos2 = vertexStr.find_first_of('/', pos + 1);
					std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - 1 - pos);
					std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);
					VertexDefine vd;
					vd.posIndex = atoi(posIndexStr.c_str());
					vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
					vd.normalIndex = atoi(normalIndexStr.c_str());
					vertexes.push_back(vd);
				}
			}
		}
	}
	delete fileContent;
	int vertexCount = (int)vertexes.size();
	mVertexBuffer = new VertexBuffer;
	mVertexBuffer->SetSize(vertexCount);
	for (int i = 0; i < vertexCount; ++i) {
		float *temp = positions[vertexes[i].posIndex - 1].v;
		mVertexBuffer->SetPosition(i, temp[0], temp[1], temp[2]);
		temp = texcoords[vertexes[i].texcoordIndex - 1].v;
		mVertexBuffer->SetTexcoord(i, temp[0], temp[1]);
		temp = normals[vertexes[i].normalIndex - 1].v;
		mVertexBuffer->SetNormal(i, temp[0], temp[1], temp[2]);
	}
	mShader = new Shader;
	mShader->Init("Res/model.vs", "Res/model.fs");
	mShader->SetVec4("U_LightPos", 0.0f, 1.0f, 1.0f, 0.0f);
	mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
	mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	mShader->SetVec4("U_LightSpecular", 1.0f, 1.0f, 1.0f, 1.0f);
	mShader->SetVec4("U_CameraPos", 0.0f, 0.0f, 0.0f, 1.0f);
	mShader->SetVec4("U_LightOpt", 32.0f, 0.0f, 0.0f, 0.0f);
	SetAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);
	SetDiffuseMaterial(0.6f, 0.6f, 0.6f, 1.0f);
	SetSpecularMaterial(1.0f, 1.0f, 1.0f, 1.0f);
}
void Model::Draw(glm::mat4 & viewMatrix, glm::mat4 projectionMatrix, float x, float y, float z) {
	mShader->SetVec4("U_CameraPos",x,y,z,1.0);
	glEnable(GL_DEPTH_TEST);
	mVertexBuffer->Bind();
	glm::mat4 it = glm::inverseTranspose(mModelMatrix);
	mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(mShader->mProgram, "IT_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(it));
	glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);
	mVertexBuffer->Unbind();
}
void Model::SetPosition(float x, float y, float z) {
	mModelMatrix = glm::translate(x, y, z);
}
void Model::SetAmbientMaterial(float r, float g, float b, float a) {
	mShader->SetVec4("U_AmbientMaterial", r, g, b, a);
}
void Model::SetDiffuseMaterial(float r, float g, float b, float a) {
	mShader->SetVec4("U_DiffuseMaterial", r, g, b, a);
}
void Model::SetSpecularMaterial(float r, float g, float b, float a) {
	mShader->SetVec4("U_SpecularMaterial", r, g, b, a);
}
void Model::SetTexture(const char*imagePath) {
	mShader->SetTexture("U_Texture", imagePath);
}