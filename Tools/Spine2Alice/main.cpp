#include "utils.h"
#include "SerializerUtils.h"
#include "SpineUtils.h"
#include "AnimationUtils.h"
#include "AttachmentUtils.h"
const char * target_folder = nullptr;
const char * target_json_file = nullptr;
const char * target_atlas_file = nullptr;
const char * user_path_prefix = nullptr;
spAtlas* atlas = nullptr;
spSkeletonData * skeletonData = nullptr;
Serializer::SpineAvatar avatar;
std::unordered_map<std::string, Serializer::Texture*> sTextures;
std::unordered_map<std::string, Serializer::GameObject*> sNodes;
Serializer::Sprite* GetSpriteInfo(const char * name,std::string&image_path) {
	for (auto iter= sTextures.begin();iter!= sTextures.end();++iter){
		Serializer::Texture*texture = iter->second;
		for (int i=0;i<texture->sprites_size();++i){
			if (strcmp(name,texture->sprites(i).name().c_str())==0){
				image_path = iter->first;
				return texture->mutable_sprites(i);
			}
		}
	}
	return nullptr;
}
void GenSlots(spSkeletonData*skeleton) {
	Serializer::GameObject*root = sNodes.find("root")->second;
	for (int i = 0; i < skeleton->slotsCount; ++i) {
		spSlotData*slot = skeleton->slots[i];
		char slot_name[256] = { 0 };
		sprintf(slot_name,"s_%s",slot->name);
		root->add_render_order(slot_name);

		Serializer::GameObject * parent_node = sNodes.find(slot->boneData->name)->second;
		Serializer::GameObject * slot_node = GetChild(parent_node);
		InitSlot(slot_node, slot_name);

		Serializer::GameObject*attachment_node = GetChild(slot_node);
		char attachment_name[256] = { 0 };
		sprintf(attachment_name, "a_%s", slot->name);
		attachment_node->set_name(attachment_name);
		attachment_node->set_layer(1);
		attachment_node->set_enable(true);
		Serializer::Transform*transform = attachment_node->mutable_transform();
		transform->add_position(0.0f); transform->add_position(0.0f); transform->add_position(0.0f);
		transform->add_rotation(0.0f); transform->add_rotation(0.0f); transform->add_rotation(0.0f);
		transform->add_scale(1.0f); transform->add_scale(1.0f); transform->add_scale(1.0f);
		transform->add_shear(0.0f); transform->add_shear(0.0f); transform->add_shear(0.0f);
		transform->set_type(0);
		auto spine_slot_data = avatar.add_slots();
		InitSlotData(slot, spine_slot_data);
	}
}
void GenAttachments(spSkeletonData*skeleton) {
	char skeleton_path[256] = { 0 };
	sprintf(skeleton_path, "%s/%s", target_folder, target_json_file);
	for (int skin_index=0;skin_index<skeleton->skinsCount;++skin_index){
		spSkin*skin = skeleton->skins[skin_index];
		InitSkinData(skeleton_path, skin->name, avatar.add_skins());
	}
}
void ExportAtlas(spAtlas*atlas) {
	spAtlasPage*current = atlas->pages;
	while (current!=nullptr){
		Serializer::Texture*texture= ConvertPageToTexture(target_folder,current);
		sTextures.insert(std::pair<std::string, Serializer::Texture*>(current->name,texture));
		ExportAssetBundle(current->name, user_path_prefix, Alice::ResourceTypePNGImage, 0, texture->SerializeAsString());
		current = current->next;
	}
}
void ExportAnimations(spSkeletonData*skeleton) {
	for (int i = 0; i < skeleton->animationsCount;++i) {
		spAnimation*animation = skeleton->animations[i];
		Serializer::Animation*animation_data = avatar.add_animations();
		animation_data->set_name(animation->name);
		animation_data->set_duration(animation->duration);
		printf("animation %s ,duration %f ,time line count : %d\n", animation->name, animation->duration, animation->timelinesCount);
		for (int ti=0;ti<animation->timelinesCount;++ti){
			spTimeline*tl = animation->timelines[ti];
			switch (tl->type){
			case SP_TIMELINE_ROTATE: {
				spRotateTimeline*rtl = SUB_CAST(spRotateTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeRotate);
				spBoneData*bone = GetBone(skeletonData,rtl->boneIndex);
				if (bone != nullptr) {
					ac->set_target(bone->name);
				}
				else {
					printf("cannot find animate target bone %d\n", bone->index);
				}
				Serializer::RotateAnimationClip*tac = ConvertRotationTimelineToAlice(tl);
				ac->set_animation_clip_data(tac->SerializeAsString());
				delete tac;
			}
				break;
			case SP_TIMELINE_TRANSLATE: {
				spTranslateTimeline*ttl = SUB_CAST(spTranslateTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeTranslate);
				spBoneData*bone = GetBone(skeletonData, ttl->boneIndex);
				if (bone != nullptr) {
					ac->set_target(bone->name);
				}
				else {
					printf("cannot find animate target bone %d\n", bone->index);
				}
				Serializer::TranslateAnimationClip*tac = ConvertTranslateTimelineToAlice(tl);
				ac->set_animation_clip_data(tac->SerializeAsString());
				delete tac;
			}
				break;
			case SP_TIMELINE_SCALE: {
				spScaleTimeline*stl = SUB_CAST(spScaleTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeScale);
				spBoneData*bone = GetBone(skeletonData, stl->boneIndex);
				if (bone != nullptr) {
					ac->set_target(bone->name);
				}
				else {
					printf("cannot find animate target bone %d\n", bone->index);
				}
				Serializer::ScaleAnimationClip*tac = ConvertScaleTimelineToAlice(tl);
				ac->set_animation_clip_data(tac->SerializeAsString());
				delete tac;
			}
				break;
			case SP_TIMELINE_SHEAR: {
				spShearTimeline*stl = SUB_CAST(spShearTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeShear);
				spBoneData*bone = GetBone(skeletonData, stl->boneIndex);
				if (bone != nullptr) {
					ac->set_target(bone->name);
				}
				else {
					printf("cannot find animate target bone %d\n", bone->index);
				}
				Serializer::ShearAnimationClip*tac = ConvertShearingTimelineToAlice(tl);
				ac->set_animation_clip_data(tac->SerializeAsString());
				delete tac;
			}
				break;
			case SP_TIMELINE_ATTACHMENT: {
				spAttachmentTimeline * atl = SUB_CAST(spAttachmentTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeSpriteSequence);
				spSlotData*slot = GetSlot(skeletonData,atl->slotIndex);
				if (slot != nullptr) {
					ac->set_target(slot->name);
					ac->set_target_component(8);
				}
				else {
					printf("cannot find animate target slot %d\n", atl->slotIndex);
				}
				Serializer::SpriteSequenceAnimationClip*ssac = ConvertAttachmentTimelineToAlice(tl);
				ac->set_animation_clip_data(ssac->SerializeAsString());
				delete ssac;
			}
				break;
			case SP_TIMELINE_COLOR: {
				spColorTimeline*ctl = SUB_CAST(spColorTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeColor);
				spSlotData*slot = GetSlot(skeletonData, ctl->slotIndex);
				if (slot != nullptr) {
					ac->set_target(slot->name);
					ac->set_target_component(8);
				}
				else {
					printf("cannot find animate target slot %d\n", ctl->slotIndex);
				}
				Serializer::ColorAnimationClip*cac = ConvertColorTimelineToAlice(tl);
				ac->set_animation_clip_data(cac->SerializeAsString());
				delete cac;
			}
				break;
			case SP_TIMELINE_DEFORM:
				break;
			case SP_TIMELINE_EVENT:
				break;
			case SP_TIMELINE_DRAWORDER: {
				spDrawOrderTimeline*stl = SUB_CAST(spDrawOrderTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeRenderOrder);
				Serializer::DrawOrderAnimationClip*tac = ConvertDrawOrderTimelineToAlice(skeletonData,tl);
				ac->set_animation_clip_data(tac->SerializeAsString());
				delete tac;
			}
				break;
			case SP_TIMELINE_IKCONSTRAINT:
				break;
			case SP_TIMELINE_TRANSFORMCONSTRAINT:
				break;
			case SP_TIMELINE_PATHCONSTRAINTPOSITION:
				break;
			case SP_TIMELINE_PATHCONSTRAINTSPACING:
				break;
			case SP_TIMELINE_PATHCONSTRAINTMIX:
				break;
			case SP_TIMELINE_TWOCOLOR:{//target must be slot
				spTwoColorTimeline*tctl = SUB_CAST(spTwoColorTimeline, tl);
				Serializer::AnimationClip * ac = animation_data->add_animation_clips();
				ac->set_type(Alice::kAnimationClipTypeTwoColor);
				spSlotData*slot = GetSlot(skeletonData, tctl->slotIndex);
				if (slot!=nullptr){
					ac->set_target(slot->name);
					ac->set_target_component(8);
				}
				else {
					printf("cannot find animate target slot %d\n",tctl->slotIndex);
				}
				Serializer::TwoColorAnimationClip*tcac=ConvertTwoColorTimelineToAlice(tl);
				ac->set_animation_clip_data(tcac->SerializeAsString());
				delete tcac;
			}
				break;
			}
		}
	}
}
int main(int argc, char **argv) {
	if (argc<4){
		return 0;
	}//argv[0]->self
	target_folder = argv[1];
	target_json_file = argv[2];
	target_atlas_file = argv[3];
	user_path_prefix = argv[4];
	char atlas_path[256] = { 0 };
	sprintf(atlas_path, "%s/%s", target_folder, target_atlas_file);
	atlas = spAtlas_createFromFile(atlas_path, nullptr);
	spSkeletonJson * json = spSkeletonJson_create(atlas);
	json->scale = 1;
	char skeleton_path[256] = { 0 };
	sprintf(skeleton_path, "%s/%s", target_folder, target_json_file);
	skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeleton_path);
	if (!skeletonData) {
		printf("%s\n", json->error);
		spSkeletonJson_dispose(json);
		exit(0);
	}
	//export alice texture with sprites
	ExportAtlas(atlas);
	//generate avatar info
	GenSkeleton(avatar, sNodes, skeletonData);
	GenSlots(skeletonData);
	GenAttachments(skeletonData);
	ExportAnimations(skeletonData);
	//write avatar info to file
	ExportAssetBundle("formated_spine.avatar", user_path_prefix, Alice::ResourceTypeAvatar, 0, avatar.SerializeAsString());
	spSkeletonJson_dispose(json);
	return 0;
}