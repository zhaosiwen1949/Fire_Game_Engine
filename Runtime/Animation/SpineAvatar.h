#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Serializer/Avatar.serializer.h"
#include "Runtime/3D/Mesh.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Geometry/AABB.h"
#include "Animation.h"
namespace Alice{
	enum AvatarType{
		kAvatarTypeSpine,
		kAvatarTypeAlice,
		kAvatarType3DKeyFrameAnimation1,//baked vertex data
		kAvatarType3DKeyFrameAnimation2,//baked animated matrix,can re target to other model
		kAvatarType3DAnimation,//baked animated curves,compute at real time,but more flexibility
		kAvatarTypeCount
	};
	enum AnimationMode{
		kAnimationModeOnce,
		kAnimationModeLoop,
		kAnimationModeCount
	};
	class GameObject;
	class SpineAvatar:public Component{
	public:
		DECLEAR_ALICE_CLASS(Avatar)
	public:
		SpineAvatar();
		virtual ~SpineAvatar();
		void Update(float deltaTime);
		void AttachAnimation(const Serializer::Animation&animation_data);
		AnimationUnit* InitTwoColorAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitColorAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitTranslateAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitRotateAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitScaleAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitShearAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitSpriteSequenceAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitRenderOrderAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		AnimationUnit* InitAnimationClip(const Serializer::AnimationClip&animation_clip_data);
		void Play(const char*name);
	public:
		const Serializer::Attachment*GetSpineSlotAttachment(const char * slot_name, const char * attachment_name);
		const Serializer::Slot*GetSpineSetupSlot(const char * slot_name);
		void ResetToTPose();
		void SetSpineAttchment(GameObject * gameobject,const char * attachment_name);
	public:
		SmartPtr<Material> mSharedMaterial;
		Serializer::SpineAvatar*mSpineAvatarData;
		std::vector<GameObject*> mRenderTargets;
		std::unordered_map<std::string, Animation*>mAnimations;//baked animations
		Animation*mCurrentAnimation;
		AnimationMode mAnimationMode;
		int mAnimationCallback;
		bool mbNotifiedAnimationEndEvent;
		void OnCurrentAnimationEnd();
		void OnCurrentAnimationEndCallback();
	};
}
