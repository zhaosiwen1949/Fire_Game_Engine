#include "EventAnimation.h"
namespace Alice {
	void EventAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
	}
	void EventAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			OnUpdateStepAnimation(start_frame);
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
		}
	}
	void EventAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			OnUpdateStepAnimation(start_frame);
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
		}
	}
}