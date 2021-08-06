#pragma once
#include "Animation.h"
#include "Runtime/Serializer/Animation.serializer.h"
namespace Alice {
	class Baked3DAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Serializer::BakedKeyFrame mKeyFrameData;
		Baked3DAnimationKeyFrame(const Serializer::BakedKeyFrame& key_frame_data);
	};
	class Baked3DAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {};
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {};
	};
}