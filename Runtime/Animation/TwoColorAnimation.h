#pragma once
#include "Runtime/Math/Vector4.h"
#include "Runtime/Serializer/Animation.serializer.h"
#include "Animation.h"

namespace Alice {
	class TwoColorAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Color4f mLightColor;
		Color4f mDarkColor;
		void Init(const Serializer::TwoColorAnimationKeyFrame&key_frame_data);
	};
	class TwoColorAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
}