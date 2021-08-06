#pragma once
#include "fbxsdk.h"
class FBXGPUDraw {
	FbxScene*mFBXScene;
	FbxManager*mFBXManager;
	FbxIOSettings*mFBXIOSetting;
	FbxArray<FbxString*> mAnimationNames;
	FbxAnimLayer*mCurrentAnimationLayer;
	FbxTime mStartTime, mStopTime, mFrameTime, mCurrentTime;
public:
	void Load(const char *filePath);
	void Draw();
	void DrawNode(FbxNode *node);
	void DrawMesh(FbxMesh *mesh, FbxAMatrix& globalPosition);
	void ExtractAllAnimationNames();
	void SetCurrentAnimation(const char *animationName);
	void ComputeKeyFrame(FbxMesh* mesh, FbxAMatrix& globalPosition, FbxVector4* pVertexArray);
	void ComputeClusterDeformation(FbxAMatrix& globalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix);
};