#include "Runtime/Scene/GameObject.h"
#include "Animation.h"
#include "Runtime/2D/ImageSprite.h"
#include "Runtime/2D/ImageSprite9.h"
#include "Runtime/String/StringUtils.h"

namespace Alice {
	AnimationUnit::AnimationUnit() : mAnimateTarget(nullptr),mKeyFrames(nullptr){
	}
	AnimationUnit::~AnimationUnit() {
		if (mKeyFrames != nullptr) {
			AnimationKeyFrame*current = mKeyFrames;
			while (current != nullptr) {
				mKeyFrames = mKeyFrames->Next<AnimationKeyFrame>();
				delete current;
				current = mKeyFrames;
			}
		}
	}
	void AnimationUnit::AddKeyFrame(AnimationKeyFrame*keyframe){
		if (mKeyFrames==nullptr){
			mKeyFrames = keyframe;
		}
		else {
			mKeyFrames->Append(keyframe);
		}
	}
	void AnimationUnit::Update(float animation_time) {
		AnimationKeyFrame *currentKeyFrame = (AnimationKeyFrame*)mKeyFrames;
		while (currentKeyFrame->mNext != nullptr && animation_time > currentKeyFrame->Next<AnimationKeyFrame>()->mTime) {
			currentKeyFrame = currentKeyFrame->Next<AnimationKeyFrame>();
		}
		if (currentKeyFrame==nullptr){
			return;
		}
		if (currentKeyFrame->mInterpolateType==kAnimationInterpolateTypeStep){
			if (currentKeyFrame->mTime <= animation_time) {
				OnUpdateStepAnimation(currentKeyFrame);
			}
		}
		else if (currentKeyFrame->mInterpolateType == kAnimationInterpolateTypeLinear) {
			if (animation_time >= currentKeyFrame->mTime) {
				OnUpdateLinearAnimation(animation_time, currentKeyFrame, currentKeyFrame->Next<AnimationKeyFrame>());
			}
		}
		else if (currentKeyFrame->mInterpolateType == kAnimationInterpolateTypeBezier) {
			if (animation_time >= currentKeyFrame->mTime) {
				OnUpdateBezierAnimation(animation_time, currentKeyFrame, currentKeyFrame->Next<AnimationKeyFrame>());
			}
		}
	}
	Animation::Animation() : mFrameCountPerSecond(60), mFrameTime(1.0f / 60.0f), mAnimationTime(0.0f),mAnimationUnits(nullptr), mDuration(0.0f){
		mName.Resize(128);
		mLastAnimationTime = 0.0f;
	}
	Animation::~Animation() {
		if (mAnimationUnits!=nullptr){
			AnimationUnit*current = mAnimationUnits;
			while (current != nullptr) {
				mAnimationUnits = mAnimationUnits->Next<AnimationUnit>();
				delete current;
				current = mAnimationUnits;
			}
		}
	}
	void Animation::SetRate(int frame_count_per_second) {
		mFrameCountPerSecond = frame_count_per_second;
		mFrameTime = 1.0f / (float)mFrameCountPerSecond;
	}
	void Animation::Reset() {
		mAnimationTime = 0.0f;
	}
	void Animation::Update(float deltaTime) {
		if (mAnimationTime >= mDuration) {
			return;
		}
		mAnimationTime += deltaTime;
		if (mAnimationTime==mLastAnimationTime){
			return;
		}
		mLastAnimationTime = mAnimationTime;
		//Debug("processing animation %f",mAnimationTime);
		AnimationUnit*current = mAnimationUnits;
		while (current != nullptr) {
			current->Update(mAnimationTime);
			current = current->Next<AnimationUnit>();
		}
	}
}
