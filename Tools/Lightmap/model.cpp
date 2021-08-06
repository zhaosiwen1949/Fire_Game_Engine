#include "model.h"
#include "utils.h"
Model::Model() {
	memset(mAmbientMaterial, 0, sizeof(mAmbientMaterial));
	memset(mDiffuseMaterial, 0, sizeof(mDiffuseMaterial));
	memset(mSpecularMaterial, 0, sizeof(mSpecularMaterial));
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
	std::vector<int> indexes;
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
				} else if (szOneLine[1] == 'n') {
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					normals.push_back(floatData);
					printf("normal : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				} else {
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					positions.push_back(floatData);
					printf("position : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				}
			}else if (szOneLine[0] == 'f'){
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
					int nCurrentVertexIndex = -1;
					int nCurrentVertexCount = (int)vertexes.size();
					for (int j = 0; j < nCurrentVertexCount; ++j) {
						if (vertexes[j].posIndex == vd.posIndex&&
							vertexes[j].normalIndex == vd.normalIndex&&
							vertexes[j].texcoordIndex == vd.texcoordIndex) {
							nCurrentVertexIndex = j;
							break;
						}
					}
					if (nCurrentVertexIndex == -1) {
						nCurrentVertexIndex = (int)vertexes.size();
						vertexes.push_back(vd);
					}
					indexes.push_back(nCurrentVertexIndex);
				}
			}
		}
	}
	mIndexCount = (int)indexes.size();
	mIndexes = new unsigned short[mIndexCount];
	for (int i = 0; i < mIndexCount; ++i) {
		mIndexes[i] = indexes[i];
	}
	int vertexCount = (int)vertexes.size();
	mVertexes = new VertexData[vertexCount];
	for (int i = 0; i < vertexCount; ++i) {
		memcpy(mVertexes[i].position, positions[vertexes[i].posIndex - 1].v, sizeof(float) * 3);
		memcpy(mVertexes[i].texcoord, texcoords[vertexes[i].texcoordIndex - 1].v, sizeof(float) * 2);
		memcpy(mVertexes[i].normal, normals[vertexes[i].normalIndex - 1].v, sizeof(float) * 3);
	}
	delete fileContent;
}
void Model::Draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mIndexCount; ++i) {
		glTexCoord2fv(mVertexes[mIndexes[i]].texcoord);
		glNormal3fv(mVertexes[mIndexes[i]].normal);
		glVertex3fv(mVertexes[mIndexes[i]].position);
	}
	glEnd();
	glPopMatrix();
}
void Model::SetAmbientMaterial(float r, float g, float b, float a) {
	mAmbientMaterial[0] = r;
	mAmbientMaterial[1] = g;
	mAmbientMaterial[2] = b;
	mAmbientMaterial[3] = a;
}
void Model::SetDiffuseMaterial(float r, float g, float b, float a) {
	mDiffuseMaterial[0] = r;
	mDiffuseMaterial[1] = g;
	mDiffuseMaterial[2] = b;
	mDiffuseMaterial[3] = a;
}
void Model::SetSpecularMaterial(float r, float g, float b, float a) {
	mSpecularMaterial[0] = r;
	mSpecularMaterial[1] = g;
	mSpecularMaterial[2] = b;
	mSpecularMaterial[3] = a;
}