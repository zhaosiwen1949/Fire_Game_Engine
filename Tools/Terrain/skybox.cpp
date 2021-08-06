#include "skybox.h"
#include "utils.h"
void SkyBox::Init(const char *imageDir) {
	mShader = new Shader[6];
	mVertexBuffer = new VertexBuffer[6];
	InitFront(imageDir);
	InitBack(imageDir);
	InitLeft(imageDir);
	InitRight(imageDir);
	InitTop(imageDir);
	InitBottom(imageDir);
}
void SkyBox::InitFront(const char *imageDir) {
	mShader[0].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "front.bmp");
	mShader[0].SetTexture("U_Texture", temp);
	mVertexBuffer[0].SetSize(4);
	mVertexBuffer[0].SetPosition(0, -0.5f, -0.5f, -0.5f);
	mVertexBuffer[0].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[0].SetPosition(1, 0.5f, -0.5f, -0.5f);
	mVertexBuffer[0].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[0].SetPosition(2, -0.5f, 0.5f, -0.5f);
	mVertexBuffer[0].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[0].SetPosition(3, 0.5f, 0.5f, -0.5f);
	mVertexBuffer[0].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::InitBack(const char *imageDir) {
	mShader[1].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "back.bmp");
	mShader[1].SetTexture("U_Texture", temp);
	mVertexBuffer[1].SetSize(4);
	mVertexBuffer[1].SetPosition(0, 0.5f, -0.5f, 0.5f);
	mVertexBuffer[1].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[1].SetPosition(1, -0.5f, -0.5f, 0.5f);
	mVertexBuffer[1].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[1].SetPosition(2, 0.5f, 0.5f, 0.5f);
	mVertexBuffer[1].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[1].SetPosition(3, -0.5f, 0.5f, 0.5f);
	mVertexBuffer[1].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::InitLeft(const char *imageDir) {
	mShader[2].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "left.bmp");
	mShader[2].SetTexture("U_Texture", temp);
	mVertexBuffer[2].SetSize(4);
	mVertexBuffer[2].SetPosition(0, -0.5f, -0.5f, 0.5f);
	mVertexBuffer[2].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[2].SetPosition(1, -0.5f, -0.5f, -0.5f);
	mVertexBuffer[2].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[2].SetPosition(2, -0.5f, 0.5f, 0.5f);
	mVertexBuffer[2].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[2].SetPosition(3, -0.5f, 0.5f, -0.5f);
	mVertexBuffer[2].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::InitRight(const char *imageDir) {
	mShader[3].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "right.bmp");
	mShader[3].SetTexture("U_Texture", temp);
	mVertexBuffer[3].SetSize(4);
	mVertexBuffer[3].SetPosition(0, 0.5f, -0.5f, -0.5f);
	mVertexBuffer[3].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[3].SetPosition(1, 0.5f, -0.5f, 0.5f);
	mVertexBuffer[3].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[3].SetPosition(2, 0.5f, 0.5f, -0.5f);
	mVertexBuffer[3].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[3].SetPosition(3, 0.5f, 0.5f, 0.5f);
	mVertexBuffer[3].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::InitTop(const char *imageDir) {
	mShader[4].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "top.bmp");
	mShader[4].SetTexture("U_Texture", temp);
	mVertexBuffer[4].SetSize(4);
	mVertexBuffer[4].SetPosition(0, -0.5f, 0.5f, -0.5f);
	mVertexBuffer[4].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[4].SetPosition(1, 0.5f, 0.5f, -0.5f);
	mVertexBuffer[4].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[4].SetPosition(2, -0.5f, 0.5f, 0.5f);
	mVertexBuffer[4].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[4].SetPosition(3, 0.5f, 0.5f, 0.5f);
	mVertexBuffer[4].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::InitBottom(const char *imageDir) {
	mShader[5].Init("Res/skybox.vs", "Res/skybox.fs");
	char temp[256];
	memset(temp, 0, 256);
	strcpy(temp, imageDir);
	strcat(temp, "bottom.bmp");
	mShader[5].SetTexture("U_Texture", temp);
	mVertexBuffer[5].SetSize(4);
	mVertexBuffer[5].SetPosition(0, -0.5f, -0.5f, 0.5f);
	mVertexBuffer[5].SetTexcoord(0, 0.0f, 0.0f);
	mVertexBuffer[5].SetPosition(1, 0.5f, -0.5f, 0.5f);
	mVertexBuffer[5].SetTexcoord(1, 1.0f, 0.0f);
	mVertexBuffer[5].SetPosition(2, -0.5f, -0.5f, -0.5f);
	mVertexBuffer[5].SetTexcoord(2, 0.0f, 1.0f);
	mVertexBuffer[5].SetPosition(3, 0.5f, -0.5f, -0.5f);
	mVertexBuffer[5].SetTexcoord(3, 1.0f, 1.0f);
}
void SkyBox::Draw(glm::mat4 &V, glm::mat4&P, float x, float y, float z) {
	glDisable(GL_DEPTH_TEST);
	mModelMatrix = glm::translate(x, y, z);
	for (int i = 0; i < 6; ++i) {
		mVertexBuffer[i].Bind();
		mShader[i].Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(V), glm::value_ptr(P));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		mVertexBuffer[i].Unbind();
	}
}