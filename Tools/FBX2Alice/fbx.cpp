#include "fbx.h"
#include "utils.h"
#pragma comment(lib,"libfbxsdk-mt.lib")
void FBX::Load(const char *filePath) {
	mFBXManager = FbxManager::Create();
	mFBXIOSetting = FbxIOSettings::Create(mFBXManager, "IOSRoot");
	FbxImporter*fbxImporter = FbxImporter::Create(mFBXManager, "");
	mFBXManager->SetIOSettings(mFBXIOSetting);
	if (fbxImporter->Initialize(filePath, -1, mFBXManager->GetIOSettings())){
		mFBXScene = FbxScene::Create(mFBXManager, "");
		fbxImporter->Import(mFBXScene);
		ExtractAllAnimationNames();
	}
	fbxImporter->Destroy();
}
void FBX::Draw() {
	mCurrentTime += mFrameTime;
	if (mCurrentTime > mStopTime) {
		mCurrentTime = mStartTime;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glTranslatef(0.0f, -.5f, -3.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.01f, 0.01f, 0.01f);
	glColor4ub(255, 255, 255, 255);
	DrawNode(mFBXScene->GetRootNode());
	glPopMatrix();
}
void FBX::DrawNode(FbxNode *node) {
	FbxMesh*mesh = node->GetMesh();
	FbxAMatrix globalPosition = node->EvaluateGlobalTransform(mCurrentTime);
	if (nullptr != mesh) {
		FbxAMatrix globalOffSet = globalPosition * GetGeometricMatrix(node);
		DrawMesh(mesh, globalOffSet);
	}
	int nChildNode = node->GetChildCount();
	for (int i = 0; i < nChildNode; ++i) {
		DrawNode(node->GetChild(i));
	}
}
void FBX::DrawMesh(FbxMesh*mesh, FbxAMatrix& globalPosition) {
	FbxVector4*controlPoints = mesh->GetControlPoints();
	FbxVector4* vertexArray = NULL;
	const int vertexCount = mesh->GetControlPointsCount();
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int clusterCount = 0;
	for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex) {
		clusterCount += ((FbxSkin *)(mesh->GetDeformer(skinIndex, FbxDeformer::eSkin)))->GetClusterCount();
	}
	if (clusterCount > 0) {
		vertexArray = new FbxVector4[vertexCount];
		memcpy(vertexArray, controlPoints, vertexCount * sizeof(FbxVector4));
		ComputeKeyFrame(mesh, globalPosition, vertexArray);
	} else {
		vertexArray = controlPoints;
	}
	const int polygonCount = mesh->GetPolygonCount();
	glBegin(GL_TRIANGLES);
	for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++) {
		const int verticeCount = mesh->GetPolygonSize(polygonIndex);
		for (int verticeIndex = 0; verticeIndex < verticeCount; verticeIndex++) {
			glVertex3dv((GLdouble *)vertexArray[mesh->GetPolygonVertex(polygonIndex, verticeIndex)]);
		}
	}
	glEnd();
	if (clusterCount > 0) {
		delete[] vertexArray;
	}
}
void FBX::ExtractAllAnimationNames() {
	mFBXScene->FillAnimStackNameArray(mAnimationNames);
	for (int i = 0; i < mAnimationNames.Size(); ++i) {
		printf("detect animation [%s]\n", mAnimationNames.GetAt(i)->Buffer());
	}
}
void FBX::SetCurrentAnimation(const char *animationName) {
	FbxAnimStack * animationStack = mFBXScene->FindMember<FbxAnimStack>(animationName);
	if (animationStack == nullptr) {
		printf("set animation %s fail\n",animationName);
		return;
	}
	mFBXScene->SetCurrentAnimationStack(animationStack);
	mCurrentAnimationLayer = mFBXScene->GetCurrentAnimationStack()->GetMember<FbxAnimLayer>();
	FbxTakeInfo* takeInfo = mFBXScene->GetTakeInfo(mFBXScene->GetCurrentAnimationStack()->GetName());
	mStartTime = takeInfo->mLocalTimeSpan.GetStart();
	mStopTime = takeInfo->mLocalTimeSpan.GetStop();
	mCurrentTime = mStartTime;
	mFrameTime.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames30);
	printf("%s-%s\n", mStartTime.GetTimeString().Buffer(), mStopTime.GetTimeString().Buffer());
}
void FBX::ComputeKeyFrame(FbxMesh* mesh, FbxAMatrix& globalPosition, FbxVector4* pVertexArray) {
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int vertexCount = mesh->GetControlPointsCount();
	FbxAMatrix* clusterDeformation = new FbxAMatrix[vertexCount];
	memset(clusterDeformation, 0, vertexCount * sizeof(FbxAMatrix));
	double* clusterWeight = new double[vertexCount];
	memset(clusterWeight, 0, vertexCount * sizeof(double));
	for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex) {
		FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(skinIndex, FbxDeformer::eSkin);
		int clusterCount = skin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex) {
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			FbxNode*bone = cluster->GetLink();
			if (nullptr == bone) continue;
			FbxAMatrix vertexTransformMatrix;
			ComputeClusterDeformation(globalPosition, mesh, cluster, vertexTransformMatrix);
			int vertexIndexCount = cluster->GetControlPointIndicesCount();
			for (int k = 0; k < vertexIndexCount; ++k) {
				int index = cluster->GetControlPointIndices()[k];
				double weight = cluster->GetControlPointWeights()[k];
				FbxAMatrix influence = vertexTransformMatrix;
				MatrixScale(influence, weight);
				MatrixAdd(clusterDeformation[index], influence);
				clusterWeight[index] += weight;
			}
		}
	}
	for (int i = 0; i < vertexCount; i++) {
		double weight = clusterWeight[i];
		if (weight != 0.0) {
			pVertexArray[i] = pVertexArray[i] * (1.0 - weight) + clusterDeformation[i].MultT(pVertexArray[i]);
		}
	}
	delete[] clusterDeformation;
	delete[] clusterWeight;
}
void FBX::ComputeClusterDeformation(FbxAMatrix& globalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix) {
	FbxAMatrix boneInitGlobalMatrix;
	FbxAMatrix boneCurrentGlobalPosition;
	pCluster->GetTransformLinkMatrix(boneInitGlobalMatrix);
	boneCurrentGlobalPosition = pCluster->GetLink()->EvaluateGlobalTransform(mCurrentTime);
	pVertexTransformMatrix = globalPosition.Inverse() * boneCurrentGlobalPosition * boneInitGlobalMatrix.Inverse() * globalPosition;
}