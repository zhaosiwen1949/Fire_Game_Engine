#include "fbxskeletoncpu.h"
#include "utils.h"
namespace Alice {
	FBXSkeletonCPU::FBXSkeletonCPU() {
		mScene = nullptr;
		mAnimationScene = nullptr;
		mMeshNode = nullptr;
		mFrameTime.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames30);
	}
	void FBXSkeletonCPU::Init(const char * path) {
		FbxManager*fbx_manager = FbxManager::Create();
		FbxIOSettings*setting = FbxIOSettings::Create(fbx_manager, IOSROOT);
		FbxImporter*fbxImporter = FbxImporter::Create(fbx_manager, "");
		fbx_manager->SetIOSettings(setting);
		if (fbxImporter->Initialize(path, -1, fbx_manager->GetIOSettings())) {
			mScene = FbxScene::Create(fbx_manager, "");
			fbxImporter->Import(mScene);
		}
		fbxImporter->Destroy();
	}
	void FBXSkeletonCPU::DumpAnimations() {
		for (int i = 0; i < mAnimationNames.Size(); ++i) {
			printf("detect animation [%s]\n", mAnimationNames.GetAt(i)->Buffer());
		}
	}
	void FBXSkeletonCPU::DrawByFBXSDK() {
		mCurrentTime += mFrameTime;
		if (mCurrentTime>mEndTime){
			mCurrentTime -= mEndTime;
		}
		DrawNodeRecursively(mScene->GetRootNode());
	}
	void FBXSkeletonCPU::DrawNodeRecursively(FbxNode*node) {
		FbxMesh*mesh = node->GetMesh();
		if (nullptr != mesh) {
			FbxAMatrix globalPosition = node->EvaluateGlobalTransform(mCurrentTime);
			FbxAMatrix globalOffSet = globalPosition * GetGeometricMatrix(node);
			DrawShape(node,globalOffSet);
		}
		int nChildNode = node->GetChildCount();
		for (int i = 0; i < nChildNode; ++i) {
			DrawNodeRecursively(node->GetChild(i));
		}
	}
	void FBXSkeletonCPU::DrawShape(FbxNode*node, FbxAMatrix& globalPosition) {
		FbxMesh*mesh = node->GetMesh();
		FbxVector4* vertexArray = NULL;
		if (HaveAnimation(mesh)){
			GetAnimatedVertices(mesh,mCurrentTime,globalPosition, &vertexArray);
		}
		else {
			GetStaticVertices(mesh, &vertexArray);
		}
		const int polygon_count = mesh->GetPolygonCount();
		double *M = (double*)globalPosition;
		float modelMatrix[] = {
			M[0],M[1],M[2],M[3],
			M[4],M[5],M[6],M[7],
			M[8],M[9],M[10],M[11],
			M[12],M[13],M[14],M[15],
		};
		glPushMatrix();
		glMultMatrixf(modelMatrix);
		glBegin(GL_TRIANGLES);
		for (int polygon_index = 0; polygon_index < polygon_count; polygon_index++) {
			const int veretex_count = mesh->GetPolygonSize(polygon_index);
			for (int vertex_index = 0; vertex_index < veretex_count; vertex_index++) {
				glVertex3dv((GLdouble *)vertexArray[mesh->GetPolygonVertex(polygon_index, vertex_index)]);
			}
		}
		glEnd();
		glPopMatrix();
		delete[] vertexArray;
	}
	void FBXSkeletonCPU::InitForGeneral(int animation_index) {
		InitSkeletonHierarchy(mScene->GetRootNode(), 0, 0, -1);
		InitTPoseMesh(mScene->GetRootNode());
		InitTPoseMeshWeight(mScene->GetRootNode());
		mScene->FillAnimStackNameArray(mAnimationNames);
		if (mAnimationNames.Size()==0){
			if (mAnimationScene!=nullptr){
				mAnimationScene->FillAnimStackNameArray(mAnimationNames);
			}
		}
		else {
			mAnimationScene = mScene;
		}
		if (mAnimationNames.Size() > 0) {
			mAnimatedFrames = new AnimatedJoint[mTPoseJoints.size()];
			mCurrentAnimateJoints = new AnimatedJointState[mTPoseJoints.size()];
			mAnimationFrameCount = 0;
			if (animation_index>=mAnimationNames.Size()){
				animation_index = 0;
			}
			FbxAnimStack * animationStack = mAnimationScene->FindMember<FbxAnimStack>(mAnimationNames.GetAt(animation_index)->Buffer());
			if (animationStack == nullptr) {
				return;
			}
			mAnimationScene->SetCurrentAnimationStack(animationStack);
			BakeAnimationFromMathDescriptionToSkeleton();
		}
	}
	void FBXSkeletonCPU::BakeAnimationFromMathDescriptionToSkeleton() {
		FbxTakeInfo* takeInfo = mAnimationScene->GetTakeInfo(mAnimationScene->GetCurrentAnimationStack()->GetName());
		mCurrentTime = takeInfo->mLocalTimeSpan.GetStart();
		mEndTime = takeInfo->mLocalTimeSpan.GetStop();
		FbxTime current_time = mCurrentTime;
		while (current_time <= mEndTime) {
			current_time += mFrameTime;
			for (auto iter=mTPoseJoints.begin();iter!=mTPoseJoints.end();++iter){
				Joint*joint = *iter;
				FbxAMatrix mesh_geometryTransform = GetGeometricMatrix(mMeshNode);
				FbxNode*bone_node = GetNodeByName(mAnimationScene->GetRootNode(),joint->mName);
				FbxAMatrix mesh_matrix = mMeshNode->EvaluateGlobalTransform(current_time);
				FbxAMatrix currentTransformOffset = mesh_matrix * mesh_geometryTransform;
				FbxAMatrix bone_animated_global_matrix = bone_node->EvaluateGlobalTransform(current_time);
				FbxAMatrix animate_global_transform = currentTransformOffset.Inverse() * bone_animated_global_matrix;
				AnimatedJointState*key_frame_state = new AnimatedJointState;
				FBXMatrixToGeneralMatrix(animate_global_transform, glm::value_ptr(key_frame_state->mGlobalTransform));
				mAnimatedFrames[joint->mIndex + 1].mFrames.push_back(key_frame_state);
			}
			mAnimationFrameCount++;
		}
	}
	void FBXSkeletonCPU::InitSkeletonHierarchy(FbxNode*node, int depth, int index, int parent_index) {
		if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && 
			node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton){
			Joint*joint=new Joint;
			joint->mIndex = index;
			joint->mParent = parent_index;
			strcpy(joint->mName, node->GetName());
			mTPoseJoints.push_back(joint);
			for (int i=0;i<depth;++i){
				printf(" ");
			}
			printf("%s %d parent(%d)\n",joint->mName,index,parent_index);
		}
		for (int childIndex = 0; childIndex < node->GetChildCount(); ++childIndex){
			FbxNode* currNode = node->GetChild(childIndex);
			InitSkeletonHierarchy(currNode, depth + 1, mTPoseJoints.size()-1, index+1);
		}
	}
	Joint*FBXSkeletonCPU::GetJointByName(const char * name) {
		for (unsigned int i = 0; i < mTPoseJoints.size(); ++i){
			if (strcmp(mTPoseJoints[i]->mName,name)==0){
				return mTPoseJoints[i];
			}
		}
		return nullptr;
	}
	Mesh*FBXSkeletonCPU::GetMeshByName(const char * name) {
		for (unsigned int i = 0; i < mMeshes.size(); ++i) {
			if (strcmp(mMeshes[i]->mName, name) == 0) {
				return mMeshes[i];
			}
		}
		return nullptr;
	}
	void FBXSkeletonCPU::InitTPoseMesh(FbxNode*node) {
		if (node->GetNodeAttribute()){
			switch (node->GetNodeAttribute()->GetAttributeType()){
			case FbxNodeAttribute::eMesh:{
				if (mMeshNode!=nullptr){
					printf("InitTPoseMesh more than one mesh node!\n");
				}
				mMeshNode = node;
				FbxMesh* current_mesh = node->GetMesh();
				
				unsigned int vertice_count = current_mesh->GetControlPointsCount();
				Mesh*mesh = new Mesh;
				strcpy(mesh->mName, node->GetName());
				mesh->mVertexCount = vertice_count;
				mesh->mVertices = new Vertice[vertice_count];
				for (unsigned int i = 0; i < vertice_count; ++i){
					mesh->mVertices[i].mVertex.mPosition[0] = static_cast<float>(current_mesh->GetControlPointAt(i).mData[0]);
					mesh->mVertices[i].mVertex.mPosition[1] = static_cast<float>(current_mesh->GetControlPointAt(i).mData[1]);
					mesh->mVertices[i].mVertex.mPosition[2] = static_cast<float>(current_mesh->GetControlPointAt(i).mData[2]);
					mesh->mVertices[i].mVertex.mPosition[3] = 1.0f;
				}
				const int polygon_count = current_mesh->GetPolygonCount();
				mesh->mTriangleCount = polygon_count;
				mesh->mTriangles = new Triangle[polygon_count];
				FbxStringList uvSetNames;
				current_mesh->GetUVSetNames(uvSetNames);
				FbxStringListItem*uvSetName = uvSetNames.GetItemAt(0);
				int vertex_counter = 0;
				for (int polygon_index = 0; polygon_index < polygon_count; polygon_index++) {
					const int veretex_count = current_mesh->GetPolygonSize(polygon_index);
					for (int vertex_index = 0; vertex_index < veretex_count; vertex_index++) {
						int vertex_index2 = current_mesh->GetPolygonVertex(polygon_index, vertex_index);
						mesh->mTriangles[polygon_index].mIndexes[vertex_index] = vertex_index2;
						FbxVector4 normal;
						if (current_mesh->GetPolygonVertexNormal(polygon_index, vertex_index, normal)) {
							mesh->mVertices[vertex_index2].mVertex.mNormal[0] = static_cast<float>(normal.mData[0]);
							mesh->mVertices[vertex_index2].mVertex.mNormal[1] = static_cast<float>(normal.mData[1]);
							mesh->mVertices[vertex_index2].mVertex.mNormal[2] = static_cast<float>(normal.mData[2]);
							mesh->mVertices[vertex_index2].mVertex.mNormal[3] = 0.0f;
						}
						FbxVector2 uv;
						bool bFlag = true;
						if (current_mesh->GetPolygonVertexUV(polygon_index, vertex_index, uvSetName->mString.Buffer(), uv, bFlag)){
							mesh->mVertices[vertex_index2].mVertex.mTexCoord0[0] = static_cast<float>(uv.mData[0]);
							mesh->mVertices[vertex_index2].mVertex.mTexCoord0[1] = static_cast<float>(uv.mData[1]);
						}
					}
				}
				mMeshes.push_back(mesh);
			}
				break;
			}
		}
		for (int i = 0; i < node->GetChildCount(); ++i){
			InitTPoseMesh(node->GetChild(i));
		}
	}
	void FBXSkeletonCPU::InitTPoseMeshWeight(FbxNode*node) {
		FbxMesh* mesh = node->GetMesh();
		if (HaveAnimation(mesh)) {
			unsigned int numOfDeformers = mesh->GetDeformerCount();
			FbxAMatrix geometryMatrix = GetGeometricMatrix(node);
			Mesh*current_mesh = GetMeshByName(node->GetName());
			for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex){
				FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
				if (!currSkin){
					continue;
				}
				unsigned int numOfClusters = currSkin->GetClusterCount();
				for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex){
					FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
					std::string currJointName = currCluster->GetLink()->GetName();
					Joint*joint = GetJointByName(currJointName.c_str());
					FbxAMatrix transformMatrix;
					FbxAMatrix transformLinkMatrix;
					FbxAMatrix globalBindposeInverseMatrix;
					currCluster->GetTransformMatrix(transformMatrix);
					currCluster->GetTransformLinkMatrix(transformLinkMatrix);
					globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryMatrix;
					FBXMatrixToGeneralMatrix(globalBindposeInverseMatrix, glm::value_ptr(joint->mTPoseInverseMatrix));
					unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
					for (unsigned int i = 0; i < numOfIndices; ++i){
						WeightPair currBlendingIndexWeightPair;
						currBlendingIndexWeightPair.mJointIndex = joint->mIndex + 1;
						currBlendingIndexWeightPair.mWeight = currCluster->GetControlPointWeights()[i];
						current_mesh->mVertices[currCluster->GetControlPointIndices()[i]].mWeights.push_back(currBlendingIndexWeightPair);
					}
				}
			}

			WeightPair currBlendingIndexWeightPair;
			currBlendingIndexWeightPair.mJointIndex = 0;
			currBlendingIndexWeightPair.mWeight = 0;
			for (int i = 0; i < current_mesh->mVertexCount; ++i) {
				for (unsigned int j = current_mesh->mVertices[i].mWeights.size(); j < 4; ++j) {
					current_mesh->mVertices[i].mWeights.push_back(currBlendingIndexWeightPair);
				}
			}
			for (int i = 0; i < current_mesh->mVertexCount; ++i) {
				WeightPair wp0 = current_mesh->mVertices[i].mWeights[0];
				WeightPair wp1 = current_mesh->mVertices[i].mWeights[1];
				WeightPair wp2 = current_mesh->mVertices[i].mWeights[2];
				WeightPair wp3 = current_mesh->mVertices[i].mWeights[3];
				//prepare for gpu version
			}
		}
		for (int i = 0; i < node->GetChildCount(); ++i) {
			InitTPoseMeshWeight(node->GetChild(i));
		}
	}
	void FBXSkeletonCPU::DrawTPose() {
		for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter) {
			Mesh*mesh = *iter;
			glPushMatrix();
			glBegin(GL_POINTS);
			const int veretex_count = mesh->mVertexCount;
			for (int vertex_index = 0; vertex_index < veretex_count; vertex_index++) {
				glVertex3fv(mesh->mVertices[vertex_index].mVertex.mPosition);
			}
			glEnd();
			glPopMatrix();
		}
	}
	void FBXSkeletonCPU::Update() {
		static int frame_index = 0;
		frame_index++;
		if (frame_index >= mAnimationFrameCount) {
			frame_index = 0;
		}
		for (int i=0;i<mTPoseJoints.size();++i){
			if (mAnimatedFrames[mTPoseJoints[i]->mIndex + 1].mFrames.size() > 0) {
				mCurrentAnimateJoints[i].mGlobalTransform = mAnimatedFrames[mTPoseJoints[i]->mIndex + 1].mFrames[frame_index]->mGlobalTransform;
			}
			else {
				mCurrentAnimateJoints[i].mGlobalTransform = mTPoseJoints[i]->mGlobalMatrix;
			}
		}
	}
	void FBXSkeletonCPU::DrawGPUVersionWithCPU() {
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		LARGE_INTEGER start;
		LARGE_INTEGER end, mCPUFreq;
		QueryPerformanceFrequency(&mCPUFreq);
		for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter) {
			QueryPerformanceCounter(&start);
			Mesh*mesh = *iter;
			VertexDataFull	* vertexes = new VertexDataFull[mesh->mVertexCount];
			for (int i = 0; i < mesh->mVertexCount; ++i) {
				glm::vec4 final_vertex(mesh->mVertices[i].mVertex.mPosition[0], mesh->mVertices[i].mVertex.mPosition[1], mesh->mVertices[i].mVertex.mPosition[2], 1.0f);
				glm::vec4 result;
				result.w = 1.0f;
				for (int j = 0; j < mesh->mVertices[i].mWeights.size(); ++j) {
					WeightPair *weight = &mesh->mVertices[i].mWeights[j];
					if (weight->mWeight==0){
						continue;
					}
					result += weight->mWeight * mCurrentAnimateJoints[weight->mJointIndex].mGlobalTransform * mTPoseJoints[weight->mJointIndex]->mTPoseInverseMatrix * final_vertex;
				}
				vertexes[i].mPosition[0] = result[0];
				vertexes[i].mPosition[1] = result[1];
				vertexes[i].mPosition[2] = result[2];
			}
			QueryPerformanceCounter(&end);
			DrawMesh(vertexes, mesh->mTriangles,mesh->mTriangleCount);
			delete[]vertexes;
		}
		glEnd();
	}
	void FBXSkeletonCPU::DrawMesh(VertexDataFull*vertexes, Triangle*triangles, int triangle_count) {
		for (int triangle_index = 0; triangle_index < triangle_count; ++triangle_index) {
			glVertex3fv(vertexes[triangles[triangle_index].mIndexes[0]].mPosition);
			glVertex3fv(vertexes[triangles[triangle_index].mIndexes[1]].mPosition);
			glVertex3fv(vertexes[triangles[triangle_index].mIndexes[2]].mPosition);
		}
	}
	void FBXSkeletonCPU::BakeTPose(const char * output_path) {
		Serializer::Avatar3D avatar;
		for (auto iter=mMeshes.begin();iter!=mMeshes.end();++iter){
			Mesh*current_mesh = *iter;
			Serializer::Mesh*mesh = avatar.add_t_pose();
			mesh->set_vertexcount(current_mesh->mVertexCount);
			mesh->set_indexcount(current_mesh->mTriangleCount * 3);
			mesh->set_name(current_mesh->mName);
			VertexDataFull	* vertexes = new VertexDataFull[current_mesh->mVertexCount];
			for (int i = 0; i < current_mesh->mVertexCount; ++i) {
				memcpy(&vertexes[i], &current_mesh->mVertices[i].mVertex, sizeof(VertexDataFull));
			}
			mesh->set_vertexbuffer(vertexes, sizeof(VertexDataFull)*current_mesh->mVertexCount);
			delete[]vertexes;
			unsigned short	* indexes = new unsigned short[current_mesh->mTriangleCount*3];
			for (int i = 0; i < current_mesh->mTriangleCount; ++i) {
				indexes[i * 3] = current_mesh->mTriangles[i].mIndexes[0];
				indexes[i * 3+1] = current_mesh->mTriangles[i].mIndexes[1];
				indexes[i * 3+2] = current_mesh->mTriangles[i].mIndexes[2];
			}
			mesh->set_indexbuffer(indexes, sizeof(unsigned short)*current_mesh->mTriangleCount * 3);
			delete[]indexes;
		}
		avatar.set_name(output_path);
		printf("avatar data size %d\n", avatar.ByteSize());
		unsigned char *szBuffer = new unsigned char[avatar.ByteSize()];
		avatar.SerializeToArray(szBuffer, avatar.ByteSize());
		Serializer::RuntimeAsset ra;
		ra.set_guid(output_path);
		ra.set_type(20);
		ra.set_rawdata(szBuffer, avatar.ByteSize());
		unsigned char *szFileContent = new unsigned char[ra.ByteSize()];
		ra.SerializeToArray(szFileContent, ra.ByteSize());
		FILE *pFile = fopen(output_path, "wb");
		fwrite(szFileContent, 1, ra.ByteSize(), pFile);
		fclose(pFile);
	}
	void FBXSkeletonCPU::BakeKeyFrame(FbxNode*node, FbxTime&currentTime,Serializer::BakedKeyFrame * key_frame, int&meshID, int targetMeshID) {
		if (meshID > targetMeshID) {
			return;
		}
		FbxMesh*mesh = node->GetMesh();
		if (nullptr != mesh) {
			if (meshID == targetMeshID) {
				Serializer::BakedMeshData*baked_mesh_data = key_frame->add_mesh_datas();
				FbxAMatrix globalPosition = node->EvaluateGlobalTransform(currentTime);
				FbxAMatrix globalOffPosition = globalPosition * GetGeometricMatrix(node);
				FbxVector4* vertexArray = NULL;
				if (HaveAnimation(mesh)) {
					GetAnimatedVertices(mesh, mCurrentTime, globalOffPosition, &vertexArray);
				}
				else {
					GetStaticVertices(mesh, &vertexArray);
				}
				const int polygon_count = mesh->GetPolygonCount();
				Mesh*current_mesh = mMeshes[targetMeshID];
				VertexDataFull	* vertexes = new VertexDataFull[current_mesh->mVertexCount];
				for (int i = 0; i < current_mesh->mVertexCount; ++i) {
					memcpy(&vertexes[i], &current_mesh->mVertices[i].mVertex, sizeof(VertexDataFull));
				}
				for (int polygon_index = 0; polygon_index < polygon_count; polygon_index++) {
					const int veretex_count = mesh->GetPolygonSize(polygon_index);
					for (int vertex_index = 0; vertex_index < veretex_count; vertex_index++) {
						int vertex_index2 = mesh->GetPolygonVertex(polygon_index, vertex_index);
						vertexes[vertex_index2].mPosition[0] = (float)vertexArray[vertex_index2].mData[0];
						vertexes[vertex_index2].mPosition[1] = (float)vertexArray[vertex_index2].mData[1];
						vertexes[vertex_index2].mPosition[2] = (float)vertexArray[vertex_index2].mData[2];
					}
				}
				baked_mesh_data->set_vertex_buffer(vertexes, sizeof(VertexDataFull)*current_mesh->mVertexCount);
				delete[] vertexArray;
				delete[] vertexes;
				double *M = (double*)globalOffPosition;
				float modelMatrix[] = {
					M[0],M[1],M[2],M[3],
					M[4],M[5],M[6],M[7],
					M[8],M[9],M[10],M[11],
					M[12],M[13],M[14],M[15],
				};
				baked_mesh_data->set_offset_matrix(modelMatrix, sizeof(float) * 16);
				meshID = meshID + 1;
				return;
			}
			meshID = meshID + 1;
		}
		int nChildNode = node->GetChildCount();
		for (int i = 0; i < nChildNode; ++i) {
			BakeKeyFrame(node->GetChild(i), currentTime, key_frame, meshID, targetMeshID);
		}
	}
	void FBXSkeletonCPU::BakeAnimationFromMathDescriptionToModel(const char * animation_name, const char * output_path) {
		Serializer::BakedAnimationData bakedAnimationData;//run or stand etc
		bakedAnimationData.set_name(animation_name);
		int nAnimatedMeshCount = mMeshes.size();
		FbxTakeInfo* takeInfo = mAnimationScene->GetTakeInfo(mAnimationScene->GetCurrentAnimationStack()->GetName());
		mCurrentTime = takeInfo->mLocalTimeSpan.GetStart();
		mEndTime = takeInfo->mLocalTimeSpan.GetStop();
		FbxTime current_time = mCurrentTime;
		int nFrameCount = 0;
		while (current_time <= mEndTime) {
			Serializer::BakedKeyFrame*key_frame = bakedAnimationData.add_key_frames();
			for (int i=0;i<nAnimatedMeshCount;++i){
				int nMeshID = 0;
				BakeKeyFrame(mScene->GetRootNode(), current_time, key_frame, nMeshID, i);
			}
			nFrameCount++;
			current_time += mFrameTime;
		}
		printf("avatar size %d %d\n", bakedAnimationData.ByteSize(),nFrameCount);
		unsigned char *szBuffer = new unsigned char[bakedAnimationData.ByteSize()];
		bakedAnimationData.SerializeToArray(szBuffer, bakedAnimationData.ByteSize());
		Serializer::RuntimeAsset ra;
		ra.set_guid(output_path);
		ra.set_type(20);
		ra.set_rawdata(szBuffer, bakedAnimationData.ByteSize());

		unsigned char *szFileContent = new unsigned char[ra.ByteSize()];
		ra.SerializeToArray(szFileContent, ra.ByteSize());
		FILE *pFile = fopen(output_path, "wb");
		fwrite(szFileContent, 1, ra.ByteSize(), pFile);
		fclose(pFile);
	}
}