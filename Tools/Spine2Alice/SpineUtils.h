#pragma once
extern "C" {
#include "spine/spine.h"
#include "spine/extension.h"
}
#include "TextureInfo.serializer.h"
#include "ImageSprite.serializer.h"
#include "Animation.serializer.h"
#include "Avatar.serializer.h"
#include "GameObject.serializer.h"
#include <unordered_map>
spSlotData*GetSlot(spSkeletonData*skeleton, int slot_index);
spBoneData*GetBone(spSkeletonData*skeleton, int bone_index);
void InitBone(Serializer::GameObject*node, spBoneData*bone);
void InitSlot(Serializer::GameObject*node, const char * name);
void GenSkeleton(Serializer::SpineAvatar&avatar, std::unordered_map<std::string, Serializer::GameObject*>&nodes, spSkeletonData*skeleton);
Serializer::Texture* ConvertPageToTexture(const char *target_folder, spAtlasPage*page);
void InitAttachmentData(spAttachment*attachment, Serializer::Attachment*attachment_data);
void InitSlotData(spSlotData*slot, Serializer::Slot*slot_data);
GLenum SpineWrapTextureFormat2GLFormat(spAtlasFormat format);
GLenum SpineWrapMode2GLWrapMode(spAtlasWrap mode);
GLenum SpineTextureFilter2GLFilter(spAtlasFilter filter);
void SpineBlendModeToGLBlendFunc(spBlendMode mode, GLenum&src_func, GLenum&dst_func);