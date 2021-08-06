#pragma once
#include "fbxsdk.h"
#include "ggl.h"
#include "fbxskeletoncpu.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
namespace Alice {
	struct GPUMesh {
		char mName[128];
		VertexBuffer*mVertexBuffer;
		IndexBuffer*mIndexBuffer;
		GPUMesh() {
			memset(mName, 0, 128);
		}
	};
	class FBXSkeletonGPU {
	public:
		FBXSkeletonGPU();
		FbxTime mCurrentTime,mFrameTime,mEndTime;
		void Init(const char * path);
		void DumpAnimations();
	public:
		FbxScene*mScene, *mAnimationScene;
		FbxArray<FbxString*> mAnimationNames;
		std::vector<Joint*> mTPoseJoints;
		std::vector<GPUMesh*> mMeshes;
		int mAnimationFrameCount;
		AnimatedJoint * mAnimatedFrames;
		AnimatedJointState * mCurrentAnimateJoints;
		FbxNode*mMeshNode;
		std::unordered_map<std::string, FbxNode*> mSceneNodes, mAnimationSceneNodes;

		Material*mMaterial;

		void Update(float delta);
		void InitForGPURendering(int animation_index = 0);
		void InitSkeletonHierarchy(FbxNode*node, int depth, int index, int parent_index);
		Joint*GetJointByName(const char * name);
		GPUMesh*GetMeshByName(const char * name);
		void InitTPoseMesh(FbxNode*node);
		void InitTPoseMeshWeight(FbxNode*node);
		void DrawGPUVersion(VkCommandBuffer commandbuffer);
		void InitAnimationX();
	};
}