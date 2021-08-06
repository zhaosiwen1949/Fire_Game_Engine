#pragma once
#include "utils.h"
#include "vertexbuffer.h"
#include "shader.h"
struct Patch {
	VertexBuffer* mVertexBuffer;
	GLuint mIBO;//lod 0,lod 1,lod 2
	glm::vec3 mPosition;
	int mCurrentLODLevel;
	int mCurrentLODLevelIndexCount;
	void Init(float xOffset, float zOffset);
	void Draw(Shader*shader);
	void UpdateIndexBuffer();
};
class Ground {
	VertexBuffer *mVertexBuffer;
	Patch* mPatches;
	Shader *mShader;
	glm::mat4 mModelMatrix;
public:
	void Init();
	void Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix);
	void Update(glm::vec3 camera_pos);
};