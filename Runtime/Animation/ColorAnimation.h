#pragma once
#include "Runtime/Math/Vector4.h"
#include "Runtime/Serializer/Animation.serializer.h"
#include "Animation.h"
namespace Alice{
	class ColorAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Color4f mColor;
		void Init(const Serializer::ColorAnimationKeyFrame&key_frame_data);
	};
	class ColorAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
}