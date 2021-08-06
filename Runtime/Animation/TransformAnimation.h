#pragma once
#include "Runtime/Math/Vector3.h"
#include "Runtime/Serializer/Animation.serializer.h"
#include "Animation.h"

namespace Alice {
	enum TransformAnimationPropertyType {
		kTransformAnimationPropertyTypeNone,
		kTransformAnimationPropertyTypeTranslate,
		kTransformAnimationPropertyTypeRotate,
		kTransformAnimationPropertyTypeScale,
		kTransformAnimationPropertyTypeShear,
		kTransformAnimationPropertyTypeCount
	};
	class TranslateAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Vector3f mPosition;
		int Init(const Serializer::TranslateAnimationKeyFrame&key_frame_data);
	};
	class RotateAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Vector3f mRotation;
		int Init(const Serializer::RotateAnimationKeyFrame&key_frame_data);
	};
	class ScaleAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Vector3f mScale;
		int Init(const Serializer::ScaleAnimationKeyFrame&key_frame_data);
	};
	class ShearAnimationKeyFrame : public AnimationKeyFrame {
	public:
		Vector3f mShear;
		int Init(const Serializer::ShearAnimationKeyFrame&key_frame_data);
	};
	class TranslateAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		void UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
	class RotateAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		void UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
	class ScaleAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		void UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
	class ShearAnimation : public AnimationUnit {
	public:
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe);
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
		void UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame);
	};
}