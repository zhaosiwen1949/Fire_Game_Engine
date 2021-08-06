#include "fbxgpudraw.h"
#include "ggl.h"
#include "utils.h"
GLuint vbo, ebo;
GLuint program;
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
#define MAX_VERTEX_COUNT 65535
void FBXGPUDraw::Load(const char *filePath) {
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
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * MAX_VERTEX_COUNT, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	modelMatrix = glm::translate(0.0f, -0.5f, -3.0f)*glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f)*glm::scale(0.01f, 0.01f, 0.01f);
	int fileSize = 0;
	unsigned char * shaderCode = LoadFileContent("Res/model.vs", fileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*)shaderCode);
	delete shaderCode;
	shaderCode = LoadFileContent("Res/model.fs", fileSize);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*)shaderCode);
	delete shaderCode;
	program = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	positionLocation = glGetAttribLocation(program, "position");
	modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
	viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
	projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
	projectionMatrix = glm::perspective(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
}
void FBXGPUDraw::Draw() {
	mCurrentTime += mFrameTime;
	if (mCurrentTime > mStopTime) {
		mCurrentTime = mStartTime;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(program);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	DrawNode(mFBXScene->GetRootNode());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}
void FBXGPUDraw::DrawNode(FbxNode *node) {
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
void FBXGPUDraw::DrawMesh(FbxMesh*mesh, FbxAMatrix& globalPosition) {
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
	for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++) {
		const int verticeCount = mesh->GetPolygonSize(polygonIndex);
		for (int verticeIndex = 0; verticeIndex < verticeCount; verticeIndex++) {
			float current_vertex[4] = {
				(float)vertexArray[mesh->GetPolygonVertex(polygonIndex, verticeIndex)][0],
				(float)vertexArray[mesh->GetPolygonVertex(polygonIndex, verticeIndex)][1],
				(float)vertexArray[mesh->GetPolygonVertex(polygonIndex, verticeIndex)][2],
				1.0f
			};
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*4*verticeIndex, sizeof(float) * 4, current_vertex);
		}
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	if (clusterCount > 0) {
		delete[] vertexArray;
	}
}
void FBXGPUDraw::ExtractAllAnimationNames() {
	mFBXScene->FillAnimStackNameArray(mAnimationNames);
	for (int i = 0; i < mAnimationNames.Size(); ++i) {
		printf("detect animation [%s]\n", mAnimationNames.GetAt(i)->Buffer());
	}
}
void FBXGPUDraw::SetCurrentAnimation(const char *animationName) {
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
void FBXGPUDraw::ComputeKeyFrame(FbxMesh* mesh, FbxAMatrix& globalPosition, FbxVector4* pVertexArray) {
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
void FBXGPUDraw::ComputeClusterDeformation(FbxAMatrix& globalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix) {
	FbxAMatrix boneInitGlobalMatrix;
	FbxAMatrix boneCurrentGlobalPosition;
	pCluster->GetTransformLinkMatrix(boneInitGlobalMatrix);
	boneCurrentGlobalPosition = pCluster->GetLink()->EvaluateGlobalTransform(mCurrentTime);
	pVertexTransformMatrix = globalPosition.Inverse() * boneCurrentGlobalPosition * boneInitGlobalMatrix.Inverse() * globalPosition;
}