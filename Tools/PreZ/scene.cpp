#include "scene.h"
#include "utils.h"
#include "model.h"
#include "camera.h"
Model model;
Camera camera;
void Init() {
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.mViewportWidth = 1280.0f;
	camera.mViewportHeight = 720.0f;
	model.Init("BakedScene.obj");
	model.mTexture = CreateTexture2DFromPNG("lightmap.png");
}
void Draw() {
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glColorMask(true, true, true, true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.SwitchTo3D();
	float frameTime = GetFrameTime();
	camera.Update(frameTime);
	//prez
	glColorMask(false, false, false, false);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	model.Draw();//

	//rendering
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);
	glColorMask(true, true, true, true);
	model.Draw();
}
void OnKeyDown(char code) {
	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = true;
		break;
	case 'D':
		camera.mbMoveRight = true;
		break;
	case 'W':
		camera.mbMoveForward = true;
		break;
	case 'S':
		camera.mbMoveBack = true;
		break;
	}
}
void OnKeyUp(char code) {
	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = false;
		break;
	case 'D':
		camera.mbMoveRight = false;
		break;
	case 'W':
		camera.mbMoveForward = false;
		break;
	case 'S':
		camera.mbMoveBack = false;
		break;
	}
}
void OnMouseMove(float deltaX, float deltaY) {
	float angleRotatedByRight = deltaY / 1000.0f;
	float angleRotatedByUp = deltaX / 1000.0f;
	camera.Yaw(-angleRotatedByUp);
	camera.Pitch(-angleRotatedByRight);
}