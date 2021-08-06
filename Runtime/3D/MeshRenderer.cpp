#include "MeshRenderer.h"
#include "Runtime/Geometry/Intersection.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/DrawCall.h"

namespace Alice {
	MeshRenderer::MeshRenderer() {
	}
	MeshRenderer::~MeshRenderer() {
		auto iter = mSubMeshRenderers.begin();
		auto iterEnd = mSubMeshRenderers.end();
		while (iterEnd != iter) {
			delete *iter;
			iter++;
		}
	}
	void MeshRenderer::SetMaterialCount(int count) {
		for (int i = 0; i < count; ++i) {
			SubMeshRenderer*mesh_renderer = new SubMeshRenderer;
			mSubMeshRenderers.push_back(mesh_renderer);
		}
	}
	void MeshRenderer::SetMaterial(int index, Material*mat) {
		mSubMeshRenderers[index]->mMaterial = mat;
	}
	void MeshRenderer::Update(float deltaTime) {
		if (mbPositionChanged) {
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			Matrix3x3 rotate_matrix(world_matrix);
			rotate_matrix.InvertTranspose();
			Matrix4x4 normal_matrix(rotate_matrix);
			for (auto iter=mSubMeshRenderers.begin();iter!=mSubMeshRenderers.end();++iter){
				SubMeshRenderer*mesh_renderer = *iter;
				mesh_renderer->mMaterial->SetMatrix4Property(UniformDefines::Matrix_M_NameInShader, world_matrix.mData);
				mesh_renderer->mMaterial->SetMatrix4Property(UniformDefines::Matrix_IT_M_NameInShader, normal_matrix.mData);
			}
			mbPositionChanged = false;
		}
	}

	void MeshRenderer::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
		Mesh*mesh = mOwner->GetComponent<Mesh>();
		if (mesh==nullptr){
			return;
		}
		for (int i=0; i<mSubMeshRenderers.size(); ++i) {
			SubMeshRenderer*mesh_renderer = mSubMeshRenderers[i];
			DrawCall*newDC = new (kMemRendererId)DrawCall;
			newDC->mMaterial = mesh_renderer->mMaterial.mPtr;
			newDC->mVBO = &mesh->mVBO;
			newDC->mEBO = &mesh->mSubMeshes[i]->mEBO;
			newDC->mGameObject = mOwner;
			newDC->mDistanceToCamera = 0.0f;
			if (newDC->mMaterial->mRenderingQueue < kRenderingQueueTransparent) {
				rq->AppendOpaqueDrawCall(newDC);
			}
			else {
				rq->AppendTransparentDrawCall(newDC);
			}
		}
	}
}
