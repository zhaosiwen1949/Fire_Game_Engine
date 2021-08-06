#pragma once
#include "vector3f.h"
class Camera{
public:
	Camera();
	Vector3f mPos,mViewCenter,mUp;
	bool mbMoveLeft, mbMoveRight, mbMoveForward, mbMoveBack;
	int mViewportWidth, mViewportHeight;
	void Update(float deltaTime);
	void Pitch(float angle);
	void Yaw(float angle);
	void RotateView(float angle, float x, float y, float z);
	void SwitchTo3D();
	void SwitchTo2D();
};