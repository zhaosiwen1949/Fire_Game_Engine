#include "TransformAnimation.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice {
	int TranslateAnimationKeyFrame::Init(const Serializer::TranslateAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mPosition.Set(key_frame_data.translation(0), key_frame_data.translation(1), key_frame_data.translation(2));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType==kAnimationInterpolateTypeBezier){
			mBezier.Init(key_frame_data.external_args(0),key_frame_data.external_args(1),key_frame_data.external_args(2),key_frame_data.external_args(3));
		}
		return kTransformAnimationPropertyTypeTranslate;
	}
	int RotateAnimationKeyFrame::Init(const Serializer::RotateAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mRotation.Set(key_frame_data.rotation(0), key_frame_data.rotation(1), key_frame_data.rotation(2));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType == kAnimationInterpolateTypeBezier) {
			mBezier.Init(key_frame_data.external_args(0), key_frame_data.external_args(1), key_frame_data.external_args(2), key_frame_data.external_args(3));
		}
		return kTransformAnimationPropertyTypeRotate;
	}
	int ScaleAnimationKeyFrame::Init(const Serializer::ScaleAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mScale.Set(key_frame_data.scale(0), key_frame_data.scale(1), key_frame_data.scale(2));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType == kAnimationInterpolateTypeBezier) {
			mBezier.Init(key_frame_data.external_args(0), key_frame_data.external_args(1), key_frame_data.external_args(2), key_frame_data.external_args(3));
		}
		return kTransformAnimationPropertyTypeScale;
	}
	int ShearAnimationKeyFrame::Init(const Serializer::ShearAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		mShear.Set(key_frame_data.shear(0), key_frame_data.shear(1), key_frame_data.shear(2));
		mInterpolateType = (AnimationInterpolateType)key_frame_data.interpolation_type();
		if (mInterpolateType == kAnimationInterpolateTypeBezier) {
			mBezier.Init(key_frame_data.external_args(0), key_frame_data.external_args(1), key_frame_data.external_args(2), key_frame_data.external_args(3));
		}
		return kTransformAnimationPropertyTypeShear;
	}
	void TranslateAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		TranslateAnimationKeyFrame*trasform_key_frame = (TranslateAnimationKeyFrame*)keyframe;
		target_gameobject->SetLocalPosition(target_gameobject->mTPoseTransform.mPosition.x + trasform_key_frame->mPosition.x,
				target_gameobject->mTPoseTransform.mPosition.y + trasform_key_frame->mPosition.y,
				target_gameobject->mTPoseTransform.mPosition.z + trasform_key_frame->mPosition.z);
	}
	void TranslateAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void TranslateAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void TranslateAnimation::UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		TranslateAnimationKeyFrame*start_key_frame = (TranslateAnimationKeyFrame*)start_frame;
		TranslateAnimationKeyFrame*end_key_frame = (TranslateAnimationKeyFrame*)end_frame;
		Vector3f pos = Lerp(start_key_frame->mPosition, end_key_frame->mPosition, percent);
			target_gameobject->SetLocalPosition(target_gameobject->mTPoseTransform.mPosition.x + pos.x,
				target_gameobject->mTPoseTransform.mPosition.y + pos.y,
				target_gameobject->mTPoseTransform.mPosition.z + pos.z);
	}
	void RotateAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		RotateAnimationKeyFrame*trasform_key_frame = (RotateAnimationKeyFrame*)keyframe;
		target_gameobject->SetLocalRotation(target_gameobject->mTPoseTransform.mRotation.x + trasform_key_frame->mRotation.x,
				target_gameobject->mTPoseTransform.mRotation.y + trasform_key_frame->mRotation.y,
				target_gameobject->mTPoseTransform.mRotation.z + trasform_key_frame->mRotation.z);
	}
	void RotateAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void RotateAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void RotateAnimation::UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		RotateAnimationKeyFrame*start_key_frame = (RotateAnimationKeyFrame*)start_frame;
		RotateAnimationKeyFrame*end_key_frame = (RotateAnimationKeyFrame*)end_frame;
		Vector3f rotation = Lerp(start_key_frame->mRotation, end_key_frame->mRotation, percent);
		target_gameobject->SetLocalRotation(target_gameobject->mTPoseTransform.mRotation.x + rotation.x,
				target_gameobject->mTPoseTransform.mRotation.y + rotation.y,
				target_gameobject->mTPoseTransform.mRotation.z + rotation.z);
	}
	void ScaleAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		ScaleAnimationKeyFrame*trasform_key_frame = (ScaleAnimationKeyFrame*)keyframe;
		target_gameobject->SetLocalScale(target_gameobject->mTPoseTransform.mScale.x * trasform_key_frame->mScale.x,
				target_gameobject->mTPoseTransform.mScale.y * trasform_key_frame->mScale.y,
				target_gameobject->mTPoseTransform.mScale.z * trasform_key_frame->mScale.z);
	}
	void ScaleAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void ScaleAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void ScaleAnimation::UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		ScaleAnimationKeyFrame*start_key_frame = (ScaleAnimationKeyFrame*)start_frame;
		ScaleAnimationKeyFrame*end_key_frame = (ScaleAnimationKeyFrame*)end_frame;
		Vector3f scale = Lerp(start_key_frame->mScale, end_key_frame->mScale, percent);
		target_gameobject->SetLocalScale(target_gameobject->mTPoseTransform.mScale.x * scale.x,
				target_gameobject->mTPoseTransform.mScale.y * scale.y,
				target_gameobject->mTPoseTransform.mScale.z * scale.z);
	}
	void ShearAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		ShearAnimationKeyFrame*trasform_key_frame = (ShearAnimationKeyFrame*)keyframe;
		target_gameobject->mLocalTransform.SetLocalShearing(target_gameobject->mTPoseTransform.mShearing.x + trasform_key_frame->mShear.x,
				target_gameobject->mTPoseTransform.mShearing.y + trasform_key_frame->mShear.y, 0.0f);
	}
	void ShearAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void ShearAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		if (end_frame == nullptr) {
			if (animation_time >= start_frame->mTime) {
				OnUpdateStepAnimation(start_frame);
			}
		}
		else {
			float deltaTime = end_frame->mTime - start_frame->mTime;
			float percent = (animation_time - start_frame->mTime) / deltaTime;
			percent = percent > 1.0f ? 1.0f : percent;
			percent = start_frame->mBezier.GetYValueFromPercent(percent);
			percent = percent > 1.0f ? 1.0f : percent;
			float src_percent = 1.0f - percent;
			UpdateProperty(src_percent, start_frame, end_frame);
		}
	}
	void ShearAnimation::UpdateProperty(float percent, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
		GameObject*target_gameobject = (GameObject*)mAnimateTarget;
		ShearAnimationKeyFrame*start_key_frame = (ShearAnimationKeyFrame*)start_frame;
		ShearAnimationKeyFrame*end_key_frame = (ShearAnimationKeyFrame*)end_frame;
		Vector3f shearing = Lerp(start_key_frame->mShear, end_key_frame->mShear, percent);
		target_gameobject->mLocalTransform.SetLocalShearing(target_gameobject->mTPoseTransform.mShearing.x + shearing.x,
				target_gameobject->mTPoseTransform.mShearing.y + shearing.y,
				target_gameobject->mTPoseTransform.mShearing.z + shearing.z);
	}
}