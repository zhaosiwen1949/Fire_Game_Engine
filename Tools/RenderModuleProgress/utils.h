#pragma once
#include "AliceGL.h"
unsigned char * LoadFileContent(const char*path, int&filesize);
GLuint CompileShader(GLenum shaderType, const char*shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
unsigned char * DecodeBMP(unsigned char*bmpFileData, int&width, int&height);
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type);
GLuint CreateTexture2DFromBMP(const char *bmpPath);
GLuint CreateProcedureTexture(int size);
GLuint CreateComputeProgram(const char* path);
GLuint CreateFeedbackProgram(const char*shaderPath, const char **values, int count,GLenum param);
struct Plane {
	float mA, mB, mC, mD;
	Plane() {

	}
	Plane(float a, float b, float c, float d) {
		mA = a;
		mB = b;
		mC = c;
		mD = d;
	}
	void SetABCD(float a, float b, float c, float d) {
		mA = a;
		mB = b;
		mC = c;
		mD = d;
	}
	void Normalize()
	{
		float invMag = 1.0f / sqrtf(mA*mA+mB*mB+mC*mC);
		mA *= invMag;
		mB *= invMag;
		mC *= invMag;
		mD *= invMag;
	}
};
struct Frustum {
	enum FrustumPlane {
		kFrustumPlaneLeft,
		kFrustumPlaneRight,
		kFrustumPlaneBottom,
		kFrustumPlaneTop,
		kFrustumPlaneNear,
		kFrustumPlaneFar,
		kFrustumPlaneCount
	};
	Plane mPlanes[kFrustumPlaneCount];
	Plane mPlanesNDC[kFrustumPlaneCount];
};
class VBO;
class Light;
struct NearFarPlanePair {
	Plane mNearPlane;
	Plane mFarPlane;
};
struct ScreenQuad {
	VBO* mVBO;
	Plane mCommonPlanes[4];
	std::vector<NearFarPlanePair*> mZSplitPlanes;
	std::unordered_set<Light*> mLights;
};
void ExtractPlane(const glm::mat4& finalMatrix, int index, float distance, Plane* out_plane);
void ExtractProjectionPlanes(const glm::mat4& finalMatrix,const Plane*frustum_space_ndc,Plane* outPlanes);
bool IntersectSphereFrustumFull(const glm::vec3& center, float radius, const Plane* p);