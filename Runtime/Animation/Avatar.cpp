#include "Avatar.h"
#include "AnimationClipType.h"
#include "Animation.h"
#include "ColorAnimation.h"
#include "Baked3DAnimation.h"
#include "EventAnimation.h"
#include "RenderOrderAnimation.h"
#include "SpriteSequenceAnimation.h"
#include "TransformAnimation.h"
#include "TwoColorAnimation.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Avatar::Avatar():mbValid(false),mTime(0.0f), mCurrentAnimation(nullptr){
		mSharedMaterial = nullptr;
		mAnimationType = kAnimationTypeRepeated;
		mAnimationCallback = LUA_REFNIL;
		mbNotifiedAnimationEndEvent = false;
		mbIsBoundingVolumeDirty = false; 
	}
	Avatar::~Avatar() {
		for (auto iter=mAnimations.begin();iter!=mAnimations.end();++iter){
			delete iter->second;
		}
		mAnimations.clear();
		if (mAnimationCallback != LUA_REFNIL) {
			lua_unref(LuaEngine::s_GlobalStatePtr, mAnimationCallback);
		}
	}
	const Sphere&Avatar::GetBoundingVolume() {
		if (mbIsBoundingVolumeDirty) {
			mbIsBoundingVolumeDirty = false;
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			mBoundingVolume.mCenter = world_matrix * mLocalBoundingVolume.mCenter;
			float scaleX = ALICE_SQRTF(ALICE_SQUARE(world_matrix.mData[0]) + ALICE_SQUARE(world_matrix.mData[1]) + ALICE_SQUARE(world_matrix.mData[2]));
			float scaleY = ALICE_SQRTF(ALICE_SQUARE(world_matrix.mData[4]) + ALICE_SQUARE(world_matrix.mData[5]) + ALICE_SQUARE(world_matrix.mData[6]));
			float scaleZ = ALICE_SQRTF(ALICE_SQUARE(world_matrix.mData[8]) + ALICE_SQUARE(world_matrix.mData[9]) + ALICE_SQUARE(world_matrix.mData[10]));
			float scale = Alice::Max(scaleX, scaleY);
			scale = Alice::Max(scale, scaleZ);
			mBoundingVolume.mRadius = mLocalBoundingVolume.mRadius*scale;
		}
		return mBoundingVolume;
	}
	void Avatar::Update(float deltaTime) {
		if (mbPositionChanged){
			mbPositionChanged = false;
			mbIsBoundingVolumeDirty = true;
		}
		if (mCurrentAnimation !=nullptr){
			mCurrentAnimation->Update(deltaTime);
			if (mCurrentAnimation->mAnimationTime >= mCurrentAnimation->mDuration) {
				OnCurrentAnimationEnd();
			}
		}
	}
	void Avatar::OnCurrentAnimationEnd() {
		if (mAnimationType == kAnimationTypeOnce) {
			if (false==mbNotifiedAnimationEndEvent){
				mbNotifiedAnimationEndEvent = true;
				OnCurrentAnimationEndCallback();
			}
		}
		else if (mAnimationType == kAnimationTypeRepeated) {
			mCurrentAnimation->mAnimationTime = 0.0f;
			OnCurrentAnimationEndCallback();
		}
	}
	void Avatar::OnCurrentAnimationEndCallback() {
		if (Lua_GetFunction(mAnimationCallback)){
			Lua_PushString("end");
			Lua_PushLString(mCurrentAnimation->mName.mText,mCurrentAnimation->mName.mLen);
			Lua_Invoke("Avatar::OnCurrentAnimationEndCallback %s");
		}
	}
	void Avatar::Play(const char*name) {
		Animation *current = nullptr;
		auto iter = mAnimations.find(name);
		if (iter != mAnimations.end()) {
			current = iter->second;
		}
		if (current != nullptr) {
			current->mAnimationTime = 0.0f;
			current->mLastAnimationTime = -1.0f;
			mbNotifiedAnimationEndEvent = false;
			mCurrentAnimation = current;
		}
	}
	void Avatar::SetMaterial(int index,Material*mat) {
		if (index<mTargets.size()){
			//mTargets[index]->SetMaterial(mat);
		}
		else {
			Error("SetMaterial failed %d(%d) %s",index,mTargets.size(),mat->mName.mText);
		}
	}
	void Avatar::AttachAnimation(const char*path) {//resource must delete manually
		Resource*res = Resource::LoadAssetWithUserPath(path);
		if (res == nullptr) {
			Error("attach animation data fail %s", path);
			return;
		}
		Serializer::BakedAnimationData animationData;
		if (animationData.ParseFromString(res->mAssetBundle->rawdata())){
			Animation*animation = new Animation;
			animation->mName=animationData.name().c_str();
			animation->mFrameCountPerSecond = 30;
			float frame_time = 1.0f / 30.0f;
			if (animationData.has_frame_rate()){
				animation->mFrameCountPerSecond = animationData.frame_rate();
				frame_time = 1.0f / float(animation->mFrameCountPerSecond);
			}
			animation->mFrameTime = frame_time;
			Baked3DAnimation*animation_clip = new Baked3DAnimation;
			animation_clip->mAnimateTarget = &mTargets;
			for (int i = 0; i < animationData.key_frames_size(); ++i) {
				const Serializer::BakedKeyFrame&baked_key_frame = animationData.key_frames(i);
				Baked3DAnimationKeyFrame *key_frame = new Baked3DAnimationKeyFrame(baked_key_frame);
				key_frame->mTime = i * frame_time;
				animation_clip->AddKeyFrame(key_frame);
			}
			animation->mAnimationUnits = animation_clip;
			animation->mDuration = animationData.key_frames_size() * frame_time;
			mAnimations.insert(std::pair<std::string, Animation*>(animationData.name(), animation));
		}
		Resource::Unload(path);
	}
	void Avatar::SetAvatarData(const char*path) {
		Resource*res = Resource::LoadAssetWithUserPath(path);
		if (res != nullptr) {
			Material*material = new Material;
			*material = *Material::mCachedMaterials["builtin/Material/SimpleColor"];
			material->mbSharedMaterial = false;
			Serializer::Avatar3D avatar;
			if (avatar.ParseFromString(res->mAssetBundle->rawdata())){
				int mesh_count = avatar.t_pose_size();
				for (int mesh_index = 0; mesh_index < mesh_count; ++mesh_index) {
					const Serializer::Mesh&mesh_data = avatar.t_pose(mesh_index);
					GameObject*go = new GameObject;
					mOwner->AppendChild(go);
					Mesh *meshComponent = new Mesh;
					meshComponent->SetOwner(go);
					//meshComponent->mMaterial = material;
					//meshComponent->SetMeshData(mesh_data);
					go->SetName(mesh_data.name().c_str());
					go->mLayer = mOwner->mLayer;
					mTargets.push_back(meshComponent);
				}
				mbValid = true;
			}
			else {
				Error("set avatar data fail %s 1", path);
			}
			Resource::Unload(path);
		}else{
			Error("set avatar data fail %s",path);
		}
	}
}
