#pragma once
#include "Runtime/Serializer/Animation.serializer.h"
#include "Animation.h"
namespace Alice {
	class SpriteSequenceAnimationKeyFrame : public AnimationKeyFrame {
	public:
		std::string mSpriteName;
		void Init(const Serializer::SpriteSequenceAnimationKeyFrame&key_frame_data);
	};
	class SpriteSequenceAnimation :public AnimationUnit {
	public:
		AliceAny mAvatar;
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {}
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {}
	};
}