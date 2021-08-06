#include "TwoColorAnimation.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/2D/ImageSprite9.h"
namespace Alice {
	void TwoColorAnimationKeyFrame::Init(const Serializer::TwoColorAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mLightColor.Set(key_frame_data.light(0), key_frame_data.light(1), key_frame_data.light(2), key_frame_data.light(3));
		mDarkColor.Set(key_frame_data.dark(0), key_frame_data.dark(1), key_frame_data.dark(2));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType == kAnimationInterpolateTypeBezier) {
			mBezier.Init(key_frame_data.external_args(0), key_frame_data.external_args(1), key_frame_data.external_args(2), key_frame_data.external_args(3));
		}
	}

	void TwoColorAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
		TwoColorAnimationKeyFrame*key_frame = (TwoColorAnimationKeyFrame*)keyframe;
		target->SetAdditionalColor(key_frame->mLightColor.x, key_frame->mLightColor.y, key_frame->mLightColor.z, key_frame->mLightColor.w);
		for (int i = 0; i < 16; ++i) {
			target->UpdateVertexTexcoord1(i, key_frame->mDarkColor.x, key_frame->mDarkColor.y, key_frame->mDarkColor.z);
		}
	}
	void TwoColorAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			OnUpdateStepAnimation(start_frame);
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
			TwoColorAnimationKeyFrame*start_key_frame = (TwoColorAnimationKeyFrame*)start_frame;
			TwoColorAnimationKeyFrame*end_key_frame = (TwoColorAnimationKeyFrame*)end_frame;
			Color4f light = Lerp(start_key_frame->mLightColor, end_key_frame->mLightColor, src_percent);
			Color4f dark = Lerp(start_key_frame->mDarkColor, end_key_frame->mDarkColor, src_percent);
			target->SetAdditionalColor(light.x, light.y, light.z, light.w);
			for (int i = 0; i < 16; ++i) {
				target->UpdateVertexTexcoord1(i, dark.x, dark.y, dark.z);
			}
		}
	}
	void TwoColorAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
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
			ImageSprite9*target = ((GameObject*)mAnimateTarget)->GetComponent<ImageSprite9>();
			TwoColorAnimationKeyFrame*start_key_frame = (TwoColorAnimationKeyFrame*)start_frame;
			TwoColorAnimationKeyFrame*end_key_frame = (TwoColorAnimationKeyFrame*)end_frame;
			Color4f light = Lerp(start_key_frame->mLightColor, end_key_frame->mLightColor, src_percent);
			Color4f dark = Lerp(start_key_frame->mDarkColor, end_key_frame->mDarkColor, src_percent);
			target->SetAdditionalColor(light.x, light.y, light.z, light.w);
			for (int i = 0; i < 16; ++i) {
				target->UpdateVertexTexcoord1(i, dark.x, dark.y, dark.z);
			}
		}
	}
}