#include "SpriteSequenceAnimation.h"
#include "SpineAvatar.h"
namespace Alice {
	void SpriteSequenceAnimationKeyFrame::Init(const Serializer::SpriteSequenceAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mSpriteName = key_frame_data.image_name();
		mInterpolateType = kAnimationInterpolateTypeStep;
	}
	void SpriteSequenceAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target = (GameObject*)mAnimateTarget;
		SpriteSequenceAnimationKeyFrame*current_key_frame = (SpriteSequenceAnimationKeyFrame*)keyframe;
		((SpineAvatar*)mAvatar)->SetSpineAttchment(target, current_key_frame->mSpriteName.c_str());
	}
}