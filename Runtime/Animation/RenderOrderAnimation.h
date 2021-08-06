#pragma once
#include "Runtime/Serializer/Animation.serializer.h"
#include "Animation.h"
namespace Alice {
	class RenderOrder;
	class RenderOrderAnimationKeyFrame : public AnimationKeyFrame {
	public:
		RenderOrder *mRenderOrder;
		void Init(AliceAny root, const Serializer::DrawOrderAnimationKeyFrame&key_frame_data);
	};
	class RenderOrderAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
}