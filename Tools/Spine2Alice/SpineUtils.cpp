#include "utils.h"
#include "SerializerUtils.h"
#include "SpineUtils.h"
spSlotData*GetSlot(spSkeletonData*skeleton, int slot_index) {
	for (int i = 0; i < skeleton->slotsCount; ++i) {
		spSlotData * slot = skeleton->slots[i];
		if (slot->index == slot_index) {
			return slot;
		}
	}
	return nullptr;
}
spBoneData*GetBone(spSkeletonData*skeleton, int bone_index) {
	for (int i = 0; i < skeleton->bonesCount; ++i) {
		spBoneData * bone = skeleton->bones[i];
		if (bone->index == bone_index) {
			return bone;
		}
	}
	return nullptr;
}
GLenum SpineTextureFilter2GLFilter(spAtlasFilter filter) {
	GLenum ret = GL_LINEAR;
	switch (filter)
	{
	case SP_ATLAS_UNKNOWN_FILTER:
		ret = GL_LINEAR;
		break;
	case SP_ATLAS_NEAREST:
		ret = GL_NEAREST;
		break;
	case SP_ATLAS_LINEAR:
		ret = GL_LINEAR;
		break;
	case SP_ATLAS_MIPMAP:
		ret = GL_MIPMAP;
		break;
	case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
		ret = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case SP_ATLAS_MIPMAP_LINEAR_NEAREST:
		ret = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
		ret = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case SP_ATLAS_MIPMAP_LINEAR_LINEAR:
		ret = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}
	return ret;
}
GLenum SpineWrapMode2GLWrapMode(spAtlasWrap mode) {
	switch (mode)
	{
	case SP_ATLAS_MIRROREDREPEAT:
		return GL_MIRRORED_REPEAT;
	case SP_ATLAS_CLAMPTOEDGE:
		return GL_CLAMP_TO_EDGE;
	case SP_ATLAS_REPEAT:
		return GL_REPEAT;
	}
	return GL_CLAMP_TO_EDGE;
}
GLenum SpineWrapTextureFormat2GLFormat(spAtlasFormat format) {
	GLenum ret = GL_RGBA;
	switch (format) {
	case SP_ATLAS_UNKNOWN_FORMAT:
		ret = GL_RGBA;
		break;
	case SP_ATLAS_ALPHA:
		ret = GL_ALPHA;
		break;
	case SP_ATLAS_INTENSITY:
		ret = GL_INTENSITY;
		break;
	case SP_ATLAS_LUMINANCE_ALPHA:
		ret = GL_LUMINANCE_ALPHA;
		break;
	case SP_ATLAS_RGB565:
		ret = GL_RGB565;
		break;
	case SP_ATLAS_RGBA4444:
		ret = GL_RGBA4;
		break;
	case SP_ATLAS_RGB888:
		ret = GL_RGB;
		break;
	case SP_ATLAS_RGBA8888:
		ret = GL_RGBA;
		break;
	}
	return ret;
}
Serializer::Texture * ConvertPageToTexture(const char *target_folder,spAtlasPage*page) {
	Serializer::Texture *texture=new Serializer::Texture;
	texture->set_path(page->name);
	texture->set_guid(page->name);
	texture->set_min_filter(SpineTextureFilter2GLFilter(page->minFilter));
	texture->set_mag_filter(SpineTextureFilter2GLFilter(page->magFilter));
	texture->set_wrap_mode_u(SpineWrapMode2GLWrapMode(page->uWrap));
	texture->set_wrap_mode_v(SpineWrapMode2GLWrapMode(page->vWrap));
	texture->set_width(page->width);
	texture->set_height(page->height);
	texture->set_format(SpineWrapTextureFormat2GLFormat(page->format));
	int file_len = 0;
	char szBuffer[256] = {0};
	sprintf(szBuffer, "%s/%s", target_folder, page->name);
	char *image_data = LoadFileContent(szBuffer, file_len);
	texture->set_image_data(image_data, file_len);
	const spAtlas*atlas = page->atlas;
	spAtlasRegion*region = atlas->regions;
	while (region!=nullptr){
		if (strcmp(region->page->name,page->name)==0){
			Serializer::Sprite*sprite = texture->add_sprites();
			if (region->index==-1){
				sprite->set_name(region->name);
			}
			else {
				char szBuffer[256] = {0};
				sprintf(szBuffer, "%s%d", region->name, region->index);
				sprite->set_name(szBuffer);
			}
			int left = region->x;
			int bottom = page->height - region->y - region->height;
			sprite->add_original(left);
			sprite->add_original(bottom);
			sprite->add_size(region->width);
			sprite->add_size(region->height);
			int offsetRight = region->originalWidth - region->width - region->offsetX;
			int	offsetTop = region->originalHeight - region->height - region->offsetY;
			if (region->rotate == 0) {
				sprite->set_rotate(false);
				sprite->add_texcoords(float(region->x) / float(page->width));
				sprite->add_texcoords(float(bottom) / float(page->height));
				sprite->add_texcoords(float(region->x + region->width) / float(page->width));
				sprite->add_texcoords(float(bottom + region->height) / float(page->height));
			}
			else {
				sprite->set_rotate(true);
				left = region->x + region->height;
				bottom = page->height - (region->y + region->width);
				float right = region->x;
				float top = page->height - region->y;
				sprite->add_texcoords(float(left) / float(page->width));
				sprite->add_texcoords(float(bottom) / float(page->height));
				sprite->add_texcoords(float(right) / float(page->width));
				sprite->add_texcoords(float(top) / float(page->height));
			}
			if (region->splits != nullptr) {
				sprite->add_split(region->splits[0] - region->offsetX);
				sprite->add_split(region->splits[1] - region->offsetY);
				sprite->add_split(region->splits[2] - offsetRight);
				sprite->add_split(region->splits[3] - offsetTop);
				if (region->pads != nullptr) {
					sprite->add_pad(region->pads[0] - region->offsetX);
					sprite->add_pad(region->pads[1] - region->offsetY);
					sprite->add_pad(region->pads[2] - offsetRight);
					sprite->add_pad(region->pads[3] - offsetTop);
				}
			}
		}
		region = region->next;
	}
	return texture;
}
void InitAttachmentData(spAttachment*attachment, Serializer::Attachment*attachment_data) {
	if (attachment->type == SP_ATTACHMENT_REGION) {
		spRegionAttachment*region_attachment = SUB_CAST(spRegionAttachment, attachment);
		Serializer::RegionAttachment spine_region_attachment_data;
		spine_region_attachment_data.add_color(region_attachment->color.r);
		spine_region_attachment_data.add_color(region_attachment->color.g);
		spine_region_attachment_data.add_color(region_attachment->color.b);
		spine_region_attachment_data.add_color(region_attachment->color.a);
		spine_region_attachment_data.add_size(region_attachment->width);
		spine_region_attachment_data.add_size(region_attachment->height);
		spine_region_attachment_data.add_translate(region_attachment->x);
		spine_region_attachment_data.add_translate(region_attachment->y);
		spine_region_attachment_data.add_rotation(region_attachment->rotation);
		spine_region_attachment_data.add_scale(region_attachment->scaleX);
		spine_region_attachment_data.add_scale(region_attachment->scaleY);
		attachment_data->set_type(SP_ATTACHMENT_REGION);
		attachment_data->set_name(attachment->name);
		attachment_data->set_data(spine_region_attachment_data.SerializeAsString());
	}
	else {
		printf("not deal with attachment %d\n", attachment->type);
	}
}
void InitSlotData(spSlotData*slot, Serializer::Slot*slot_data) {
	char slot_name[256] = { 0 };
	sprintf(slot_name, "s_%s", slot->name);
	slot_data->set_name(slot_name);
	if (slot->attachmentName!=nullptr){
		slot_data->set_attachment(slot->attachmentName);
	}
	slot_data->add_color(slot->color.r);
	slot_data->add_color(slot->color.g);
	slot_data->add_color(slot->color.b);
	slot_data->add_color(slot->color.a);
	if (slot->darkColor != nullptr) {
		slot_data->add_dark(slot->darkColor->r);
		slot_data->add_dark(slot->darkColor->g);
		slot_data->add_dark(slot->darkColor->b);
		slot_data->add_dark(slot->darkColor->a);
	}
	GLenum src_func, dst_func;
	SpineBlendModeToGLBlendFunc(slot->blendMode, src_func, dst_func);
	slot_data->set_blend_func_src(src_func);
	slot_data->set_blend_func_dst(dst_func);
}
void InitBone(Serializer::GameObject*node, spBoneData*bone) {
	char fixed_bone_name[256] = { 0 };
	sprintf(fixed_bone_name, "b_%s", bone->name);
	node->set_name(fixed_bone_name);
	node->set_layer(1);
	node->set_enable(true);
	auto transform=node->mutable_transform();
	transform->set_type(bone->transformMode);
	transform->add_position(bone->x); transform->add_position(bone->y); transform->add_position(0.0f);
	transform->add_rotation(0.0f); transform->add_rotation(0.0f); transform->add_rotation(bone->rotation);
	transform->add_scale(bone->scaleX); transform->add_scale(bone->scaleY); transform->add_scale(1.0f);
	transform->add_shear(bone->shearX); transform->add_shear(bone->shearY); transform->add_shear(0.0f);
}
void InitSlot(Serializer::GameObject*node, const char * name) {
	node->set_name(name);
	node->set_layer(1);
	node->set_enable(true);
	auto transform = node->mutable_transform();
	transform->set_type(0);
	transform->add_position(0.0f); transform->add_position(0.0f); transform->add_position(0.0f);
	transform->add_rotation(0.0f); transform->add_rotation(0.0f); transform->add_rotation(0.0f);
	transform->add_scale(1.0f); transform->add_scale(1.0f); transform->add_scale(1.0f);
	transform->add_shear(0.0f); transform->add_shear(0.0f); transform->add_shear(0.0f);
}
void GenSkeleton(Serializer::SpineAvatar&avatar, std::unordered_map<std::string, Serializer::GameObject *>&nodes, spSkeletonData*skeleton) {
	for (int i = 0; i < skeleton->bonesCount; ++i) {
		spBoneData*bone = skeleton->bones[i];
		Serializer::GameObject * node = nullptr;
		const spBoneData *parent = bone->parent;
		if (parent != nullptr) {
			auto iter_parent = nodes.find(parent->name);
			node = GetChild(iter_parent->second);
		}
		else {
			node = avatar.mutable_setup_pose();
		}
		InitBone(node, bone);
		Serializer::Bone*bone_state = avatar.add_bones();
		bone_state->add_translate(node->transform().position(0));
		bone_state->add_translate(node->transform().position(1));
		bone_state->add_rotation(0.0f);
		bone_state->add_rotation(0.0f);
		bone_state->add_rotation(node->transform().rotation(2));
		bone_state->add_scale(node->transform().scale(0));
		bone_state->add_scale(node->transform().scale(1));
		bone_state->add_shear(node->transform().shear(0));
		bone_state->add_shear(node->transform().shear(1));
		bone_state->set_name(node->name());
		nodes.insert(std::pair<std::string, Serializer::GameObject*>(bone->name, node));
	}
}
void SpineBlendModeToGLBlendFunc(spBlendMode mode, GLenum&src_func, GLenum&dst_func) {
	enum BlendMode {
		BlendMode_Normal = 0,
		BlendMode_Additive,
		BlendMode_Multiply,
		BlendMode_Screen
	};
	switch (mode) {
	case BlendMode_Additive:
		src_func = GL_SRC_ALPHA;
		dst_func = GL_ONE;
		break;
	case BlendMode_Multiply:
		src_func = GL_DST_COLOR;
		dst_func = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case BlendMode_Screen:
		src_func = GL_ONE;
		dst_func = GL_ONE_MINUS_SRC_COLOR;
		break;
	default:
		src_func = GL_SRC_ALPHA;
		dst_func = GL_ONE_MINUS_SRC_ALPHA;
		break;
	}
}