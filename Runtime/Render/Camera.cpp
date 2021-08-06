#include "Runtime/Render/Camera.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Scene/GameObject.h"
#include "Material.h"

namespace Alice
{
	void ExtractProjectionPlanes(const Matrix4x4& finalMatrix, Plane* outPlanes){
		float tmpVec[4];
		float otherVec[4];
		tmpVec[0] = finalMatrix.Get(3, 0);//M41
		tmpVec[1] = finalMatrix.Get(3, 1);//M42
		tmpVec[2] = finalMatrix.Get(3, 2);//M43
		tmpVec[3] = finalMatrix.Get(3, 3);//M44
		otherVec[0] = finalMatrix.Get(0, 0);//M11
		otherVec[1] = finalMatrix.Get(0, 1);//M12
		otherVec[2] = finalMatrix.Get(0, 2);//M13
		otherVec[3] = finalMatrix.Get(0, 3);//M14
		// left & right
		outPlanes[kFrustumPlaneLeft].SetABCD(otherVec[0] + tmpVec[0], otherVec[1] + tmpVec[1], otherVec[2] + tmpVec[2], otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneLeft].Normalize();
		outPlanes[kFrustumPlaneRight].SetABCD(-otherVec[0] + tmpVec[0], -otherVec[1] + tmpVec[1], -otherVec[2] + tmpVec[2], -otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneRight].Normalize();
		// bottom & top
		otherVec[0] = finalMatrix.Get(1, 0);//M21
		otherVec[1] = finalMatrix.Get(1, 1);//M22
		otherVec[2] = finalMatrix.Get(1, 2);//M23
		otherVec[3] = finalMatrix.Get(1, 3);//M24
		outPlanes[kFrustumPlaneBottom].SetABCD(otherVec[0] + tmpVec[0], otherVec[1] + tmpVec[1], otherVec[2] + tmpVec[2], otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneBottom].Normalize();
		outPlanes[kFrustumPlaneTop].SetABCD(-otherVec[0] + tmpVec[0], -otherVec[1] + tmpVec[1], -otherVec[2] + tmpVec[2], -otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneTop].Normalize();
		otherVec[0] = finalMatrix.Get(2, 0);//M31
		otherVec[1] = finalMatrix.Get(2, 1);//M32
		otherVec[2] = finalMatrix.Get(2, 2);//M33
		otherVec[3] = finalMatrix.Get(2, 3);//M34
		// near & far
		outPlanes[kFrustumPlaneNear].SetABCD(otherVec[0] + tmpVec[0], otherVec[1] + tmpVec[1], otherVec[2] + tmpVec[2], otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneNear].Normalize();
		outPlanes[kFrustumPlaneFar].SetABCD(-otherVec[0] + tmpVec[0], -otherVec[1] + tmpVec[1], -otherVec[2] + tmpVec[2], -otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneFar].Normalize();
	}
	Camera::Camera(){
		mFieldOfView = 45.0f;
		mNearClip = 0.1f;
		mFarClip = 1000.0f;
		mCameraMode = PerspectiveMode;
		mCullingFlags = Alice::GameObjectLayer_Default;
		mClearColor.Set(41, 77, 121, 255);
		mDepth = 0;
		mClearFlags = kClearModeClearAll;
        mbEnable=true;
		mOrthoModeSize = 100.0f;
		mAspect = 0.0f;
		mbOffScreen = false;
		mFBO = nullptr;
		mbRenderDepth = false;
		mDepthFBO = nullptr;
		mSkyBox = nullptr;
	}

	void Camera::ClearBackground(){
		if (kClearModeSkybox==mClearFlags){
			OGL_CALL(glClearColor(mClearColor.r / 255.0f, mClearColor.g / 255.0f, mClearColor.b / 255.0f, mClearColor.a / 255.0f));
			OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			
		}else if (kClearModeSolidColor==mClearFlags){
			OGL_CALL(glClearColor(mClearColor.r / 255.0f, mClearColor.g / 255.0f, mClearColor.b / 255.0f, mClearColor.a / 255.0f));
			OGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		}else if (kClearModeClearAll==mClearFlags){
			OGL_CALL(glClearColor(mClearColor.r / 255.0f, mClearColor.g / 255.0f, mClearColor.b / 255.0f, mClearColor.a / 255.0f));
			OGL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
		}else if (kClearModeDepthOnly==mClearFlags){
			OGL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
		}
	}
	void Camera::LookAt(Vector3f eye, Vector3f lookAt, Vector3f up){
		mViewMatrix.LookAt(eye, lookAt, up);
	}
	void Camera::SetProjection(float fovy, float aspect, float zNear, float zFar){
		mProjectionMatrix.SetPerspective(fovy, aspect, zNear, zFar);
	}

	void Camera::SetClearColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a){
		mClearColor.r = r;
		mClearColor.g = g;
		mClearColor.b = b;
		mClearColor.a = a;
	}

	void Camera::OnViewPortSizeChange(int width, int height)
	{
        //Debug("on view port size change %d %d",width,height);
		mViewWidth = (float)width;
		mViewHeight = (float)height;
		mAspect = (float)width / (float)height;
		if (mCameraMode==PerspectiveMode)
		{
			SetProjection(mFieldOfView, mAspect, mNearClip, mFarClip);
		}
		else if(mCameraMode==OrthoMode)
		{
			mOrthoModeSize = width;
			mResolutionWidth = mOrthoModeSize;
			mResolutionHeight = mResolutionWidth / mAspect;
			//Debug("aspect %f,resolution width %f resolution height %f",mAspect,mResolutionWidth,mResolutionHeight);
			mProjectionMatrix.SetOrtho(-mResolutionWidth /2.0f, mResolutionWidth / 2.0f, -mResolutionHeight/2.0f, mResolutionHeight/2.0f, mNearClip, mFarClip);
		}
		if(mOwner!=nullptr)
			mOwner->OnViewportChanged(mViewWidth,mViewHeight);
	}

	bool Camera::GetViewRect(Rect<float>&rect) {
		if (mCameraMode==OrthoMode){
			rect.mLeft = -mResolutionWidth / 2.0f;
			rect.mWidth = mResolutionWidth;
			rect.mBottom = -mResolutionHeight / 2.0f;
			rect.mHeight = mResolutionHeight;
			return true;
		}
		return false;
	}
	void Camera::ScreenCoordToOpenGLCoord(float & x, float & y) {
		float full_resolution_width = GetSceneManager()->mFullResolutionWidth;
		float full_resolution_height = GetSceneManager()->mFullResolutionHeight;
		float scale = mViewWidth / full_resolution_width;
		x = scale * (x - full_resolution_width / 2.0);
		y = scale * (full_resolution_height /2.0 - y);
	}
	Camera::~Camera(){
		if (mbOffScreen){
			GetSceneManager()->RemoveOffScreenCamera(this);
		}else {
			GetSceneManager()->RemoveCamera(this);
		}
		if (mFBO != nullptr) {
			delete mFBO;
		}
		if (mDepthFBO!=nullptr){
			delete mDepthFBO;
		}
	}

	void Camera::Update(float deltaTime){
		if (mbPositionChanged) {
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			Vector3f pos(world_matrix.mData[12], world_matrix.mData[13], world_matrix.mData[14]);
			Vector3f up(world_matrix.mData[4], world_matrix.mData[5], world_matrix.mData[6]);
			Vector3f d(-world_matrix.mData[8], -world_matrix.mData[9], -world_matrix.mData[10]);
			Vector3f center = pos + d;
			mViewMatrix.LookAt(pos, center, up);
			mbPositionChanged = false;
		}
	}
	int Camera::GetScreenShot(unsigned char **out, int&width, int&height) {
		if (mFBO != nullptr) {
			width = mFBO->mWidth;
			height = mFBO->mHeight;
			*out = new unsigned char[width *height * 4];
			unsigned char *pixel = *out;
			mFBO->Switch();
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			mFBO->Restore();
			for (int i = 0; i < width*height * 4; i += 4) {
				unsigned char temp = pixel[i];
				pixel[i] = pixel[i + 2];
				pixel[i + 2] = temp;
			}
			return 4;
		}
		else {
			width = 256;
			height = 256;
			*out = new unsigned char[width *height * 4];
			unsigned char *pixel = *out;
			memset(pixel, 0, width*height * 4);
			return 4;
		}
	}
}
