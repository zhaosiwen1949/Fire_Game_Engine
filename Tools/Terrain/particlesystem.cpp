#include "particlesystem.h"
#include "utils.h"
void ParticleSystem::Init(float x, float y, float z) {
	mModelMatrix = glm::translate(x, y, z);
	mVertexBuffer = new VertexBuffer;
	int particleCount = 180;
	mVertexBuffer->SetSize(particleCount);
	for (int i = 0; i < particleCount; ++i) {
		mVertexBuffer->SetPosition(i, 2.0f*cosf(float(i) * 8.0f*3.14f / 180.0f), 0.0f, 2.0f*sinf(float(i) * 8.0f*3.14f / 180.0f));
		mVertexBuffer->SetColor(i, 0.1f, 0.4f, 0.6f);
	}
	mShader = new Shader;
	mShader->Init("Res/particle.vs", "Res/particle.fs");
	mShader->SetTexture("U_Texture", CreateProcedureTexture(128));
}
void ParticleSystem::Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix) {
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	mVertexBuffer->Bind();
	mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
	glDrawArrays(GL_POINTS, 0, mVertexBuffer->mVertexCount);
	mVertexBuffer->Unbind();
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_PROGRAM_POINT_SIZE);
}
void ParticleSystem::Update(float deltaTime) {
	static float angle = 0.0f;
	angle += deltaTime*10.0f;
	mModelMatrix = glm::rotate(angle, 0.0f, 1.0f, 0.0f);
	for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
		Vertex &vertex = mVertexBuffer->Get(i);
		vertex.Normal[1] = 0.1f*i;
		if (i > 90) {
			break;
		}
	}
}