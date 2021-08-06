#include "SpineAvatar.h"
#include "AnimationClipType.h"
#include "Animation.h"
#include "ColorAnimation.h"
#include "EventAnimation.h"
#include "RenderOrderAnimation.h"
#include "SpriteSequenceAnimation.h"
#include "TransformAnimation.h"
#include "TwoColorAnimation.h"
#include "Runtime/2D/ImageSprite9.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	SpineAvatar::SpineAvatar():mCurrentAnimation(nullptr){
		mAnimationMode = kAnimationModeOnce;
		mAnimationCallback = LUA_REFNIL;
		mbNotifiedAnimationEndEvent = false;
	}
	SpineAvatar::~SpineAvatar() {
		for (auto iter=mAnimations.begin();iter!=mAnimations.end();++iter){
			delete iter->second;
		}
		mAnimations.clear();
		if (mAnimationCallback != LUA_REFNIL) {
			lua_unref(LuaEngine::s_GlobalStatePtr, mAnimationCallback);
		}
	}
	void SpineAvatar::Update(float deltaTime) {
		if (mCurrentAnimation !=nullptr){
			mCurrentAnimation->Update(deltaTime);
			if (mCurrentAnimation->mAnimationTime >= mCurrentAnimation->mDuration) {
				OnCurrentAnimationEnd();
			}
		}
	}
	void SpineAvatar::OnCurrentAnimationEnd() {
		if (mAnimationMode == kAnimationModeOnce) {
			if (false==mbNotifiedAnimationEndEvent){
				mbNotifiedAnimationEndEvent = true;
				OnCurrentAnimationEndCallback();
			}
		}
		else if (mAnimationMode == kAnimationModeLoop) {
			mCurrentAnimation->mAnimationTime = 0.0f;
			OnCurrentAnimationEndCallback();
		}
	}
	void SpineAvatar::OnCurrentAnimationEndCallback() {
		return;
		if (Lua_GetFunction(mAnimationCallback)){
			Lua_PushString("end");
			Lua_PushLString(mCurrentAnimation->mName.mText,mCurrentAnimation->mName.mLen);
			Lua_Invoke("SpineAvatar::OnCurrentAnimationEndCallback %s");
		}
	}
	void SpineAvatar::Play(const char*name) {
		Animation *current = nullptr;
		auto iter = mAnimations.find(name);
		if (iter != mAnimations.end()) {
			current = iter->second;
		}
		if (current != nullptr) {
			current->mAnimationTime = 0.0f;
			current->mLastAnimationTime = -1.0f;
			mbNotifiedAnimationEndEvent = false;
			if (mCurrentAnimation!=nullptr){
				ResetToTPose();
			}
			mCurrentAnimation = current;
		}
	}
	AnimationUnit*SpineAvatar::InitTwoColorAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		if (animation_clip_data.target_component() == ImageSprite9::ClassID) {
			char szBuffer[256] = { 0 };
			sprintf(szBuffer, "a_%s", animation_clip_data.target().c_str());
			GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
			TwoColorAnimation * two_colo_animation_clip = new TwoColorAnimation;//two color animation clip
			two_colo_animation_clip->mAnimateTarget = target_object;
			Serializer::TwoColorAnimationClip stcac;
			if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
				for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
					TwoColorAnimationKeyFrame*key_frame = new TwoColorAnimationKeyFrame;
					key_frame->Init(stcac.key_frames(animation_key_frame_index));
					two_colo_animation_clip->AddKeyFrame(key_frame);
				}
			}
			return two_colo_animation_clip;
		}
		return nullptr;
	}
	AnimationUnit*SpineAvatar::InitColorAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		if (animation_clip_data.target_component() == ImageSprite9::ClassID) {
			char szBuffer[256] = { 0 };
			sprintf(szBuffer, "a_%s", animation_clip_data.target().c_str());
			GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
			ColorAnimation * color_animation_clip = new ColorAnimation;
			color_animation_clip->mAnimateTarget = target_object;
			Serializer::ColorAnimationClip stcac;
			if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
				for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
					ColorAnimationKeyFrame*key_frame = new ColorAnimationKeyFrame;
					key_frame->Init(stcac.key_frames(animation_key_frame_index));
					color_animation_clip->AddKeyFrame(key_frame);
				}
			}
			return color_animation_clip;
		}
		return nullptr;
	}
	AnimationUnit*SpineAvatar::InitSpriteSequenceAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		if (animation_clip_data.target_component() == ImageSprite9::ClassID) {
			char szBuffer[256] = { 0 };
			sprintf(szBuffer, "a_%s", animation_clip_data.target().c_str());
			GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
			SpriteSequenceAnimation * sprite_sequence_animation_clip = new SpriteSequenceAnimation;
			sprite_sequence_animation_clip->mAnimateTarget = target_object;
			sprite_sequence_animation_clip->mAvatar = this;
			Serializer::SpriteSequenceAnimationClip stcac;
			if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
				for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
					SpriteSequenceAnimationKeyFrame*key_frame = new SpriteSequenceAnimationKeyFrame;
					key_frame->Init(stcac.key_frames(animation_key_frame_index));
					sprite_sequence_animation_clip->AddKeyFrame(key_frame);
				}
			}
			return sprite_sequence_animation_clip;
		}
		return nullptr;
	}
	AnimationUnit*SpineAvatar::InitTranslateAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		char szBuffer[256] = { 0 };
		sprintf(szBuffer, "b_%s", animation_clip_data.target().c_str());
		GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
		TranslateAnimation * translate_animation_clip = new TranslateAnimation;
		translate_animation_clip->mAnimateTarget = target_object;
		Serializer::TranslateAnimationClip stcac;
		if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
			for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
				TranslateAnimationKeyFrame*key_frame = new TranslateAnimationKeyFrame;
				key_frame->Init(stcac.key_frames(animation_key_frame_index));
				translate_animation_clip->AddKeyFrame(key_frame);
			}
		}
		return translate_animation_clip;
	}
	AnimationUnit*SpineAvatar::InitRotateAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		char szBuffer[256] = { 0 };
		sprintf(szBuffer, "b_%s", animation_clip_data.target().c_str());
		GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
		RotateAnimation * rotate_animation_clip = new RotateAnimation;
		rotate_animation_clip->mAnimateTarget = target_object;
		Serializer::RotateAnimationClip stcac;
		if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
			for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
				RotateAnimationKeyFrame*key_frame = new RotateAnimationKeyFrame;
				key_frame->Init(stcac.key_frames(animation_key_frame_index));
				rotate_animation_clip->AddKeyFrame(key_frame);
			}
		}
		return rotate_animation_clip;
	}
	AnimationUnit*SpineAvatar::InitScaleAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		char szBuffer[256] = { 0 };
		sprintf(szBuffer, "b_%s", animation_clip_data.target().c_str());
		GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
		ScaleAnimation * scale_animation_clip = new ScaleAnimation;
		scale_animation_clip->mAnimateTarget = target_object;
		Serializer::ScaleAnimationClip stcac;
		if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
			for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
				ScaleAnimationKeyFrame*key_frame = new ScaleAnimationKeyFrame;
				key_frame->Init(stcac.key_frames(animation_key_frame_index));
				scale_animation_clip->AddKeyFrame(key_frame);
			}
		}
		return scale_animation_clip;
	}
	AnimationUnit*SpineAvatar::InitShearAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		char szBuffer[256] = { 0 };
		sprintf(szBuffer, "b_%s", animation_clip_data.target().c_str());
		GameObject * target_object = ((GameObject*)mOwner)->FindChild(szBuffer);
		ShearAnimation * shear_animation_clip = new ShearAnimation;
		shear_animation_clip->mAnimateTarget = target_object;
		Serializer::ShearAnimationClip stcac;
		if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
			for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
				ShearAnimationKeyFrame*key_frame = new ShearAnimationKeyFrame;
				key_frame->Init(stcac.key_frames(animation_key_frame_index));
				shear_animation_clip->AddKeyFrame(key_frame);
			}
		}
		return shear_animation_clip;
	}
	AnimationUnit*SpineAvatar::InitRenderOrderAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		RenderOrderAnimation * render_order_animation_clip = new RenderOrderAnimation;
		Serializer::DrawOrderAnimationClip stcac;
		render_order_animation_clip->mAnimateTarget = mOwner;
		if (stcac.ParseFromArray(animation_clip_data.animation_clip_data().c_str(), animation_clip_data.animation_clip_data().length())) {
			for (int animation_key_frame_index = 0; animation_key_frame_index < stcac.key_frames_size(); ++animation_key_frame_index) {
				RenderOrderAnimationKeyFrame*key_frame = new RenderOrderAnimationKeyFrame;
				key_frame->Init(mOwner,stcac.key_frames(animation_key_frame_index));
				render_order_animation_clip->AddKeyFrame(key_frame);
			}
		}
		return render_order_animation_clip;
	}
	AnimationUnit*SpineAvatar::InitAnimationClip(const Serializer::AnimationClip&animation_clip_data) {
		AnimationClipType animation_clip_type = (AnimationClipType)animation_clip_data.type();
		switch (animation_clip_type){
		case Alice::kAnimationClipTypeTwoColor:
			return InitTwoColorAnimationClip(animation_clip_data);
		case Alice::kAnimationClipTypeTranslate:
			return InitTranslateAnimationClip(animation_clip_data);
		case kAnimationClipTypeRotate:
			return InitRotateAnimationClip(animation_clip_data);
		case kAnimationClipTypeScale:
			return InitScaleAnimationClip(animation_clip_data);
		case kAnimationClipTypeShear:
			return InitShearAnimationClip(animation_clip_data);
		case Alice::kAnimationClipTypeSpriteSequence:
			return InitSpriteSequenceAnimationClip(animation_clip_data);
		case Alice::kAnimationClipTypeColor:
			return InitColorAnimationClip(animation_clip_data);
		case Alice::kAnimationClipTypeRenderOrder:
			return InitRenderOrderAnimationClip(animation_clip_data);
		}
		return nullptr;
	}
	void SpineAvatar::AttachAnimation(const Serializer::Animation&animation_data) {
		if (mAnimations.find(animation_data.name())!=mAnimations.end()){
			Error("Attach Animation failed %s already exist",animation_data.name().c_str());
			return;
		}
		Animation*animation = new Animation;
		for (int animation_clip_index = 0; animation_clip_index < animation_data.animation_clips_size(); ++animation_clip_index) {
			AnimationUnit*animation_clip = InitAnimationClip(animation_data.animation_clips(animation_clip_index));
			if (animation_clip != nullptr) {
				if (animation->mAnimationUnits == nullptr) {
					animation->mAnimationUnits = animation_clip;
				}
				else {
					animation->mAnimationUnits->Append(animation_clip);
				}
			}
		}
		animation->mDuration = animation_data.duration();
		animation->mName = animation_data.name().c_str();
		mAnimations.insert(std::pair<std::string, Animation*>(animation_data.name(), animation)); 
	}
	void SpineAvatar::SetSpineAttchment(GameObject * gameobject, const char * attachment_name) {
		const Serializer::Attachment*attachment = GetSpineSlotAttachment(gameobject->Parent<GameObject>()->mName.mText, attachment_name);
		if (attachment != nullptr) {
			if (attachment->type() == 0) {//region
				ImageSprite9*is = gameobject->GetComponent<ImageSprite9>();
				if (is == nullptr) {
					is = new ImageSprite9;
					is->SetOwner(gameobject);
				}
				is->SetMaterial(mSharedMaterial.mPtr);
				const Serializer::Slot*setup_slot = GetSpineSetupSlot(gameobject->Parent<GameObject>()->mName.mText);
				is->BlendFunc(setup_slot->blend_func_src(), setup_slot->blend_func_dst());
				Serializer::RegionAttachment spine_region_attachment;
				if (spine_region_attachment.ParseFromString(attachment->data())) {
					is->SetSpriteByName(attachment->name().c_str());
					is->SetSize(spine_region_attachment.size(0), spine_region_attachment.size(1));
					is->SetColor(spine_region_attachment.color(0), spine_region_attachment.color(1), spine_region_attachment.color(2), spine_region_attachment.color(3));
					is->SetAdditionalColor(setup_slot->color(0), setup_slot->color(1), setup_slot->color(2), setup_slot->color(3));
					if (setup_slot->dark_size() > 0) {
						for (int i = 0; i < 16; ++i) {
							is->UpdateVertexTexcoord1(i, setup_slot->dark(0), setup_slot->dark(1), setup_slot->dark(2), setup_slot->dark(3));
						}
					}
					gameobject->SetLocalPosition(spine_region_attachment.translate(0), spine_region_attachment.translate(1), 0.0f);
					gameobject->SetLocalRotation(0.0f, 0.0f, spine_region_attachment.rotation(0));
					gameobject->SetLocalScale(spine_region_attachment.scale(0), spine_region_attachment.scale(1), 1.0f);
				}
				gameobject->mbEnable = true;
			}
			else {
				Error("unkown attachment type : %d", attachment->type());
			}
		}
		else {
			if (strcmp(attachment_name, "alice_none") == 0) {
				gameobject->mbEnable = false;
			}
			else {
				Error("SetSpineAttchment cannot find %s->%s", gameobject->Parent<GameObject>()->mName.mText, attachment_name);
			}
		}
	}
	const Serializer::Attachment*SpineAvatar::GetSpineSlotAttachment(const char * slot_name, const char * attachment_name) {
		if (mSpineAvatarData == nullptr) {
			return nullptr;
		}
		for (int skin_index=0;skin_index< mSpineAvatarData->skins_size();++skin_index){
			const Serializer::Skin&spine_skin = mSpineAvatarData->skins(skin_index);
			for (int slot_index=0;slot_index<spine_skin.slot_available_attachments_info_size();++slot_index){
				const Serializer::SlotAvailableAttachmentsInfo&slot_attachments = spine_skin.slot_available_attachments_info(slot_index);
				if (strcmp(slot_name,slot_attachments.name().c_str())==0){
					for (int slot_attachment_index = 0; slot_attachment_index < slot_attachments.available_attachments_size(); ++slot_attachment_index) {
						const Serializer::Attachment*attachment = &slot_attachments.available_attachments(slot_attachment_index);
						if (strcmp(attachment->name().c_str(), attachment_name) == 0) {
							return attachment;
						}
					}
				}
			}
		}
		return nullptr;
	}
	const Serializer::Slot*SpineAvatar::GetSpineSetupSlot(const char * slot_name) {
		if (mSpineAvatarData == nullptr) {
			return nullptr;
		}
		for (int setup_slot_index = 0; setup_slot_index < mSpineAvatarData->slots_size(); ++setup_slot_index) {
			const Serializer::Slot& setup_slot = mSpineAvatarData->slots(setup_slot_index);
			if (strcmp(setup_slot.name().c_str(), slot_name) == 0) {
				return &setup_slot;
			}
		}
		return nullptr;
	}
	void SpineAvatar::ResetToTPose() {
		if (mSpineAvatarData != nullptr) {
			bool first_time_to_init = mRenderTargets.empty();
			for (int setup_bone_index = 0; setup_bone_index < mSpineAvatarData->bones_size(); ++setup_bone_index) {
				const Serializer::Bone& setup_bone = mSpineAvatarData->bones(setup_bone_index);
				GameObject*bone_game_object = nullptr;
				if (strcmp(setup_bone.name().c_str(),"b_root")==0){
				}
				else {
					bone_game_object = mOwner->FindChild(setup_bone.name().c_str());
					bone_game_object->SetLocalPosition(setup_bone.translate(0), setup_bone.translate(1), 0.0f);
					bone_game_object->SetLocalRotation(0.0f, 0.0f, setup_bone.rotation(0));
					bone_game_object->SetLocalScale(setup_bone.scale(0), setup_bone.scale(1), 1.0f);
					bone_game_object->mLocalTransform.SetLocalShearing(setup_bone.shear(0), setup_bone.shear(1), 0.0f);
				}
			}
			for (int setup_slot_index = 0; setup_slot_index < mSpineAvatarData->slots_size(); ++setup_slot_index) {
				const Serializer::Slot& setup_slot = mSpineAvatarData->slots(setup_slot_index);
				GameObject*slot_game_object = mOwner->FindChild(setup_slot.name().c_str());
				GameObject*attachment_object = slot_game_object->Child<GameObject>();
				if (first_time_to_init) {
					mRenderTargets.push_back(attachment_object);
				}
				attachment_object->SetLocalPosition(0.0f, 0.0f, 0.0f);
				attachment_object->SetLocalRotation(0.0f, 0.0f, 0.0f);
				attachment_object->SetLocalScale(1.0f, 1.0f, 1.0f);
				attachment_object->mLocalTransform.SetLocalShearing(0.0f, 0.0f, 0.0f);
				if (setup_slot.has_attachment()){
					attachment_object->mbEnable = true;
					SetSpineAttchment(attachment_object, setup_slot.attachment().c_str());
				}
				else {
					attachment_object->mbEnable = false;
				}
			}
		}
	}
}
