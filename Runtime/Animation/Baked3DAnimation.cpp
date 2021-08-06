#include "Baked3DAnimation.h"
#include "Runtime/3D/Mesh.h"
namespace Alice{
	Baked3DAnimationKeyFrame::Baked3DAnimationKeyFrame(const Serializer::BakedKeyFrame&  key_frame_data) {
		mKeyFrameData = key_frame_data;
		mInterpolateType = kAnimationInterpolateTypeStep;
	}
	void Baked3DAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		std::vector<Mesh*>* targets = (std::vector<Mesh*>*)mAnimateTarget;
		Baked3DAnimationKeyFrame*key_frame_data = (Baked3DAnimationKeyFrame*)keyframe;
		for (int i=0;i<key_frame_data->mKeyFrameData.mesh_datas_size();++i){
			const Serializer::BakedMeshData & mesh_data = key_frame_data->mKeyFrameData.mesh_datas(i);
			//(*targets)[i]->SetVertexData(mesh_data.vertex_buffer().c_str(), mesh_data.vertex_buffer().size());
			//(*targets)[i]->AnimateModelMatrix(mesh_data.offset_matrix().c_str());
		}
	}
}