#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Serializer/Avatar.serializer.h"
#include "Runtime/3D/Mesh.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Geometry/AABB.h"
#include "Animation.h"
namespace Alice{
	class GameObject;
	class Avatar:public Component{
	public:
		DECLEAR_ALICE_CLASS(Avatar)
	public:
		Avatar();
		virtual ~Avatar();
		void Update(float deltaTime);
		void SetMaterial(int index,Material*mat);
		void SetAvatarData(const char*path);//init avatar -> T pose
		void AttachAnimation(const char*path);//animation data only
		void Play(const char*name);
	public:
		std::vector<GameObject*> mRenderTargets;
		std::unordered_map<std::string, Animation*>mAnimations;//baked animations
		Animation*mCurrentAnimation;
		AnimationType mAnimationType;
		int mAnimationCallback;
		bool mbNotifiedAnimationEndEvent;
		void OnCurrentAnimationEnd();
		void OnCurrentAnimationEndCallback();

		SmartPtr<Material> mSharedMaterial;

		std::vector<Mesh*> mTargets;
		bool mbValid;
		float mTime, mCurrentKeyFrameGAPTime;
		int mCurrentAnimationFrameCount, mCurrentFrame;
		int mCurrentAnimationFrameCountPerSecond;
	public:
		const Sphere&GetBoundingVolume();
		bool mbIsBoundingVolumeDirty;
		Sphere mBoundingVolume;
		Sphere mLocalBoundingVolume;
	};
}
