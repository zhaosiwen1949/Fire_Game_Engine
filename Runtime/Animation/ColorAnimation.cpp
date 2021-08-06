#include "ColorAnimation.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/2D/ImageSprite9.h"
namespace Alice{
	void ColorAnimationKeyFrame::Init(const Serializer::ColorAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mColor.Set(key_frame_data.color(0), key_frame_data.color(1), key_frame_data.color(2), key_frame_data.color(3));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType == kAnimationInterpolateTypeBezier) {
			mBezier.Init(key_frame_data.external_args(0), key_frame_data.external_args(1), key_frame_data.external_args(2), key_frame_data.external_args(3));
		}
	}

	void ColorAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
		ColorAnimationKeyFrame*current_key_frame = (ColorAnimationKeyFrame*)keyframe;
		target->SetAdditionalColor(current_key_frame->mColor.x, current_key_frame->mColor.y, current_key_frame->mColor.z, current_key_frame->mColor.w);
	}
	void ColorAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			OnUpdateStepAnimation(start_frame);
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			ColorAnimationKeyFrame*start_key_frame = (ColorAnimationKeyFrame*)start_frame;
			ColorAnimationKeyFrame*end_key_frame = (ColorAnimationKeyFrame*)end_frame;
			Color4f color = Lerp(start_key_frame->mColor, end_key_frame->mColor, src_percent);
			ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
			target->SetAdditionalColor(color.x, color.y, color.z, color.w);
		}
	}
	void ColorAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			OnUpdateStepAnimation(start_frame);
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			ColorAnimationKeyFrame*start_key_frame = (ColorAnimationKeyFrame*)start_frame;
			ColorAnimationKeyFrame*end_key_frame = (ColorAnimationKeyFrame*)end_frame;
			Color4f color = Lerp(start_key_frame->mColor, end_key_frame->mColor, src_percent);
			ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
			target->SetAdditionalColor(color.x, color.y, color.z, color.w);
		}
	}
}