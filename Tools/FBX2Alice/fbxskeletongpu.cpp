#include "fbxskeletongpu.h"
#include "utils.h"
namespace Alice {
	FBXSkeletonGPU::FBXSkeletonGPU() {
		mScene = nullptr;
		mAnimationScene = nullptr;
		mMeshNode = nullptr;
		mFrameTime.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames24);
	}
	void FBXSkeletonGPU::Init(const char * path) {
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
	void FBXSkeletonGPU::DumpAnimations() {
		for (int i = 0; i < mAnimationNames.Size(); ++i) {
			printf("detect animation [%s]\n", mAnimationNames.GetAt(i)->Buffer());
		}
	}
	void FBXSkeletonGPU::InitForGPURendering(int animation_index) {
		InitSkeletonHierarchy(mScene->GetRootNode(), 0, 0, -1);
		InitTPoseMesh(mScene->GetRootNode());
		InitTPoseMeshWeight(mScene->GetRootNode());
		mScene->FillAnimStackNameArray(mAnimationNames);
		if (mAnimationNames.Size() == 0) {
			if (mAnimationScene != nullptr) {
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
			if (animation_index >= mAnimationNames.Size()) {
				animation_index = 0;
			}
			FbxAnimStack * animationStack = mAnimationScene->FindMember<FbxAnimStack>(mAnimationNames.GetAt(animation_index)->Buffer());
			if (animationStack == nullptr) {
				return;
			}
			mAnimationScene->SetCurrentAnimationStack(animationStack);
			InitAnimationX();
		}
		for (auto iter=mMeshes.begin();iter!=mMeshes.end();++iter){
			GPUMesh*mesh = *iter;
			mesh->mVertexBuffer->SubmitData();
			mesh->mIndexBuffer->SubmitData();
		}
	}
	void FBXSkeletonGPU::InitAnimationX() {
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
	void FBXSkeletonGPU::InitSkeletonHierarchy(FbxNode*node, int depth, int index, int parent_index) {
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
	Joint*FBXSkeletonGPU::GetJointByName(const char * name) {
		for (unsigned int i = 0; i < mTPoseJoints.size(); ++i){
			if (strcmp(mTPoseJoints[i]->mName,name)==0){
				return mTPoseJoints[i];
			}
		}
		return nullptr;
	}
	GPUMesh*FBXSkeletonGPU::GetMeshByName(const char * name) {
		for (unsigned int i = 0; i < mMeshes.size(); ++i) {
			if (strcmp(mMeshes[i]->mName, name) == 0) {
				return mMeshes[i];
			}
		}
		return nullptr;
	}
	void FBXSkeletonGPU::InitTPoseMesh(FbxNode*node) {
		if (node->GetNodeAttribute()){
			switch (node->GetNodeAttribute()->GetAttributeType()){
			case FbxNodeAttribute::eMesh: {
				if (mMeshNode != nullptr) {
					printf("InitTPoseMesh more than one mesh node!\n");
				}
				mMeshNode = node;
				FbxMesh* current_mesh = node->GetMesh();

				unsigned int vertice_count = current_mesh->GetControlPointsCount();
				GPUMesh*mesh = new GPUMesh;
				strcpy(mesh->mName, node->GetName());
				mesh->mVertexBuffer = new VertexBuffer;
				mesh->mVertexBuffer->SetSize(vertice_count);
				for (unsigned int i = 0; i < vertice_count; ++i) {
					mesh->mVertexBuffer->SetPosition(i, static_cast<float>(current_mesh->GetControlPointAt(i).mData[0]),
						static_cast<float>(current_mesh->GetControlPointAt(i).mData[1]), static_cast<float>(current_mesh->GetControlPointAt(i).mData[2]));
				}
				const int polygon_count = current_mesh->GetPolygonCount();
				mesh->mIndexBuffer = new IndexBuffer;
				mesh->mIndexBuffer->SetSize(polygon_count * 3);
				FbxStringList uvSetNames;
				current_mesh->GetUVSetNames(uvSetNames);
				FbxStringListItem*uvSetName = uvSetNames.GetItemAt(0);
				int vertex_counter = 0;
				for (int polygon_index = 0; polygon_index < polygon_count; polygon_index++) {
					const int veretex_count = current_mesh->GetPolygonSize(polygon_index);
					for (int vertex_index = 0; vertex_index < veretex_count; vertex_index++) {
						int vertex_index2 = current_mesh->GetPolygonVertex(polygon_index, vertex_index);
						mesh->mIndexBuffer->AppendIndex(vertex_index2);
						FbxVector4 normal;
						if (current_mesh->GetPolygonVertexNormal(polygon_index, vertex_index, normal)) {
							mesh->mVertexBuffer->SetNormal(vertex_index2,static_cast<float>(normal.mData[0]),static_cast<float>(normal.mData[1]),
								static_cast<float>(normal.mData[2]));
						}
						FbxVector2 uv;
						bool bFlag = true;
						if (current_mesh->GetPolygonVertexUV(polygon_index, vertex_index, uvSetName->mString.Buffer(), uv, bFlag)) {
							mesh->mVertexBuffer->SetTexcoord(vertex_index2, static_cast<float>(uv.mData[0]), static_cast<float>(uv.mData[1]),0.0f,0.0f);
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
	void FBXSkeletonGPU::InitTPoseMeshWeight(FbxNode*node) {
		FbxMesh* mesh = node->GetMesh();
		if (HaveAnimation(mesh)) {
			unsigned int numOfDeformers = mesh->GetDeformerCount();
			FbxAMatrix geometryMatrix = GetGeometricMatrix(node);
			GPUMesh*current_mesh = GetMeshByName(node->GetName());
			std::vector<std::vector<WeightPair>> weights;
			for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex){
				FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
				if (!currSkin){
					continue;
				}
				weights.resize(current_mesh->mVertexBuffer->mVertexCount);
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
						int vertice_index = currCluster->GetControlPointIndices()[i];
						weights[vertice_index].push_back(currBlendingIndexWeightPair);
					}
				}
			}
			WeightPair currBlendingIndexWeightPair;
			currBlendingIndexWeightPair.mJointIndex = 0;
			currBlendingIndexWeightPair.mWeight = 0;
			for (int i = 0; i< current_mesh->mVertexBuffer->mVertexCount; ++i){
				for (unsigned int j = weights[i].size(); j < 4; ++j){
					weights[i].push_back(currBlendingIndexWeightPair);
				}
			}
			for (int i = 0; i < current_mesh->mVertexBuffer->mVertexCount; ++i) {
				WeightPair wp0 = weights[i][0];
				WeightPair wp1 = weights[i][1];
				WeightPair wp2 = weights[i][2];
				WeightPair wp3 = weights[i][3];
				current_mesh->mVertexBuffer->SetTexcoord(i, wp0.mJointIndex, wp1.mJointIndex, wp2.mJointIndex, wp3.mJointIndex);
				current_mesh->mVertexBuffer->SetNormal(i, wp0.mWeight, wp1.mWeight, wp2.mWeight, wp3.mWeight);
			}
		}
		for (int i = 0; i < node->GetChildCount(); ++i) {
			InitTPoseMeshWeight(node->GetChild(i));
		}
	}
	void FBXSkeletonGPU::Update(float delta) {
		static int frame_index = 0;
		static float animation_time = 0.0f;
		static float frame_time = 0.033f;
		animation_time += delta;
		if (animation_time-frame_time>0.0f){
			animation_time -= frame_time;
			frame_index++;
		}
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
	void FBXSkeletonGPU::DrawGPUVersion(VkCommandBuffer commandbuffer) {
		for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter){
			GPUMesh*mesh = *iter;
			xSetDynamicState(mMaterial->mFixedPipeline, commandbuffer);
			VkBuffer vertexbuffers[] = { mesh->mVertexBuffer->mBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				mMaterial->mFixedPipeline->mPipeline);
			vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexbuffers, offsets);
			vkCmdBindIndexBuffer(commandbuffer, mesh->mIndexBuffer->mBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				mMaterial->mFixedPipeline->mPipelineLayout, 0, 1, &mMaterial->mProgram.mDescriptorSet,
				0, nullptr);
			vkCmdDrawIndexed(commandbuffer, mesh->mIndexBuffer->mIndexCount, 1, 0, 0, 0);
		}
	}
}