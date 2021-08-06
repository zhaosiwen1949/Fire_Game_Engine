#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Geometry/CubicBezier.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Serializer/Animation.serializer.h"
#include <list>
namespace Alice{
	enum AnimatePropertyType {
		kAnimatePropertyTypeTransform,
		kAnimatePropertyTypeColor,
		kAnimatePropertyTypeSpriteSequence,
		kAnimatePropertyTypeRenderOrder,
		kAnimatePropertyTypeCount
	};
	enum AnimationInterpolateType{
		kAnimationInterpolateTypeLinear,
		kAnimationInterpolateTypeStep,
		kAnimationInterpolateTypeBezier,
		kAnimationInterpolateTypeCount
	};
	class AnimationKeyFrame : public DoubleLinkedList{
	public:
		float mTime;
		CubicBezier mBezier;
		AnimationInterpolateType mInterpolateType;
	};
	enum AnimationType {
		kAnimationTypeRepeated = 0,
		kAnimationTypeOnce = 1 << 1,
		kAnimationTypePingpong = 1 << 2,
	};
	enum AnimationState {
		AnimationStatePlaying,
		AnimationStatePause,
		AnimationStateStop
	};
	//internal use to update animation,not show up to user
	class AnimationUnit : public DoubleLinkedList {
	public:
		AliceAny mAnimateTarget;
		AnimationKeyFrame*mKeyFrames;
		AnimationUnit();
		~AnimationUnit();
		void Update(float animation_time);
		virtual void OnUpdateStepAnimation(AnimationKeyFrame*keyframe) = 0;
		virtual void OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame,AnimationKeyFrame*end_frame) = 0;
		virtual void OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) = 0;
		virtual void AddKeyFrame(AnimationKeyFrame*keyframe);
	};
	class Animation{
	public:
		float mFrameTime;
		float mLastAnimationTime;
		float mAnimationTime;
		float mDuration;
		int mFrameCountPerSecond;//动画的刷新频率
		AliceAny mAnimateTarget;
		FixedString mName;
		AnimationUnit * mAnimationUnits;
		Animation();
		~Animation();
		void SetRate(int frame_count_per_second);
		void Reset();
		void Update(float deltaTime);
	};
}