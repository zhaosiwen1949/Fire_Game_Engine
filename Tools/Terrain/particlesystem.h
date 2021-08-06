#pragma once
#include "vertexbuffer.h"
#include "shader.h"
class ParticleSystem{
	VertexBuffer *mVertexBuffer;
	glm::mat4 mModelMatrix;
	Shader *mShader;
public:
	void Init(float x, float y, float z);
	void Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix);
	void Update(float deltaTime);
};