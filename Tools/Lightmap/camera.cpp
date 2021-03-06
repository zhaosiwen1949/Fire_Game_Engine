#include "camera.h"
Camera::Camera():mPos(10.0f,8.0f,10.0f),mViewCenter(0.0f,0.0f,0.0f),mUp(0.0f,1.0f,0.0f){
	mbMoveLeft = false;
	mbMoveRight = false;
	mbMoveForward = false;
	mbMoveBack = false;
}
void Camera::Update(float deltaTime){
	float moveSpeed = 10.0f;
	Vector3f forwardDirection = mViewCenter - mPos;
	forwardDirection.Normalize();
	Vector3f rightDirection = Cross(forwardDirection, mUp);
	rightDirection.Normalize();
	if (mbMoveLeft) {
		Vector3f delta = rightDirection*deltaTime*moveSpeed;
		mPos = mPos - delta;
		mViewCenter = mViewCenter - delta;
	}
	if (mbMoveRight) {
		Vector3f delta = rightDirection*deltaTime*moveSpeed;
		mPos = mPos + delta;
		mViewCenter = mViewCenter + delta;
	}
	if (mbMoveForward) {
		Vector3f delta = forwardDirection*deltaTime*moveSpeed;
		mPos = mPos + delta;
		mViewCenter = mViewCenter + delta;
	}
	if (mbMoveBack) {
		Vector3f delta = forwardDirection*deltaTime*moveSpeed;
		mPos = mPos - delta;
		mViewCenter = mViewCenter - delta;
	}
	glLoadIdentity();
	gluLookAt(mPos.x, mPos.y, mPos.z,
		mViewCenter.x, mViewCenter.y, mViewCenter.z,
		mUp.x, mUp.y, mUp.z);
}
void Camera::Pitch(float angle) {
	Vector3f viewDirection = mViewCenter - mPos;
	viewDirection.Normalize();
	Vector3f rightDirection = Cross(viewDirection, mUp);
	rightDirection.Normalize();
	RotateView(angle, rightDirection.x, rightDirection.y, rightDirection.z);
}
void Camera::Yaw(float angle) {
	RotateView(angle, mUp.x, mUp.y, mUp.z);
}
void Camera::RotateView(float angle, float x, float y, float z) {
	Vector3f viewDirection = mViewCenter - mPos;
	Vector3f newDirection(0.0f, 0.0f, 0.0f);
	float C = cosf(angle);
	float S = sinf(angle);
	Vector3f tempX(C + x*x*(1 - C), x*y*(1 - C) - z*S, x*z*(1 - C) + y*S);
	newDirection.x = tempX*viewDirection;
	Vector3f tempY(x*y*(1 - C) + z*S, C + y*y*(1 - C), y*z*(1 - C) - x*S);
	newDirection.y = tempY*viewDirection;
	Vector3f tempZ(x*z*(1 - C) - y*S, y*z*(1 - C) + x*S, C + z*z*(1 - C));
	newDirection.z = tempZ*viewDirection;
	mViewCenter = mPos + newDirection;
}
void Camera::SwitchTo3D() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, (float)mViewportWidth / (float)mViewportHeight, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Camera::SwitchTo2D() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-mViewportWidth / 2, mViewportWidth / 2, -mViewportHeight / 2, mViewportHeight / 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}