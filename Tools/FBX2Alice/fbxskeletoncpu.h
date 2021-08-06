#pragma once
#include "fbxsdk.h"
#include "ggl.h"
#include "Runtime/Serializer/Animation.serializer.h"
#include "Runtime/Serializer/Avatar.serializer.h"
#include "Runtime/Serializer/Assets.serializer.h"
namespace Alice {
	struct VertexDataFull {//ES2.0 8
		float mPosition[4];//16bytes
		float mTexCoord0[4];//16 bytes
		float mNormal[4];//16bytes
		float mTangent[4];//16 bytes
		float mTexCoord1[4];//16 bytes
	};
	struct WeightPair {
		int mJointIndex;
		float mWeight;
	};
	struct Vertice {
		VertexDataFull mVertex;
		std::vector<WeightPair> mWeights;
	};
	struct Triangle {
		int mIndexes[3];
	};
	struct Joint {
		char mName[128];
		int mIndex;
		int mParent;
		glm::mat4 mGlobalMatrix;
		glm::mat4 mTPoseInverseMatrix;
		std::vector<Joint*> mKeyFrames;
		Joint() {
			memset(mName, 0, 128);
		}
	};
	struct Mesh {
		char mName[128];
		Vertice * mVertices;
		Triangle * mTriangles;
		int mVertexCount;
		int mTriangleCount;
		Mesh() {
			memset(mName, 0, 128);
		}
	};
	struct AnimatedJointState {
		glm::mat4 mGlobalTransform;
	};
	struct AnimatedJoint {
		std::vector<AnimatedJointState*> mFrames;
	};
	class FBXSkeletonCPU {
	public:
		FBXSkeletonCPU();
		FbxTime mCurrentTime,mFrameTime,mEndTime;
		void Init(const char * path);
		void DumpAnimations();
	public:
		void DrawByFBXSDK();
		void DrawNodeRecursively(FbxNode*node);
		void DrawShape(FbxNode*node, FbxAMatrix& globalPosition);
	public:
		FbxScene*mScene, *mAnimationScene;
		FbxArray<FbxString*> mAnimationNames;
		std::vector<Joint*> mTPoseJoints;
		std::vector<Mesh*> mMeshes;
		int mAnimationFrameCount;
		AnimatedJoint * mAnimatedFrames;
		AnimatedJointState * mCurrentAnimateJoints;
		FbxNode*mMeshNode;
		std::unordered_map<std::string, FbxNode*> mSceneNodes, mAnimationSceneNodes;

		void Update();
		void InitForGeneral(int animation_index = 0);
		void InitSkeletonHierarchy(FbxNode*node, int depth, int index, int parent_index);
		Joint*GetJointByName(const char * name);
		Mesh*GetMeshByName(const char * name);
		void InitTPoseMesh(FbxNode*node);
		void InitTPoseMeshWeight(FbxNode*node);
		void DrawTPose();
		void DrawGPUVersionWithCPU();
		void DrawMesh(VertexDataFull*vertexes,Triangle*triangles,int triangle_count);
		void BakeAnimationFromMathDescriptionToSkeleton();
		void BakeKeyFrame(FbxNode*node, FbxTime&currentTime,Serializer::BakedKeyFrame *animData, int&meshID, int targetMeshID);
		void BakeAnimationFromMathDescriptionToModel(const char * animation_name,const char * output_path);
		void BakeTPose(const char * output_path);
	};
}