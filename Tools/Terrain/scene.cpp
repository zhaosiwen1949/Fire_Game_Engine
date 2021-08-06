#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "shader.h"
#include "model.h"
#include "skybox.h"
#include "particlesystem.h"
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
glm::vec3 cameraPos(10.0f,10.0f,10.0f),camera_target(0.0f,0.0f,0.0f);
Ground ground;
SkyBox skybox;
static bool sMoveForward = false, sMoveBackward = false;
void UpdateCamera(float deltaTime) {
	float speed = 2.0f;
	glm::vec3 camera_direction = camera_target - cameraPos;
	glm::vec3 move_vector = camera_direction * speed * deltaTime;
	if (sMoveForward) {
		cameraPos = cameraPos + move_vector;
		camera_target = camera_target + move_vector;
	}
	if (sMoveBackward) {
		cameraPos = cameraPos - move_vector;
		camera_target = camera_target - move_vector;
	}
	viewMatrix = glm::lookAt(cameraPos, camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
	ground.Update(cameraPos);
}
void Init() {
	viewMatrix = glm::lookAt(cameraPos, camera_target, glm::vec3(0.0f,1.0f,0.0f));
	ground.Init();
	skybox.Init("Res/");
}
void SetViewPortSize(float width, float height) {
	projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}
void Draw() {
	float frameTime = GetFrameTime();
	UpdateCamera(frameTime);
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	glPolygonMode(GL_FRONT, GL_LINE);
	ground.Draw(viewMatrix, projectionMatrix);
	glPolygonMode(GL_FRONT, GL_FILL);
}
void OnKeyDown(int key_code) {
	switch (key_code) {
	case 'W':
		sMoveForward = true;
		break;
	case 'S':
		sMoveBackward = true;
	}
}
void OnKeyUp(int key_code) {
	switch (key_code) {
	case 'W':
		sMoveForward = false;
		break;
	case 'S':
		sMoveBackward = false;
	}
}