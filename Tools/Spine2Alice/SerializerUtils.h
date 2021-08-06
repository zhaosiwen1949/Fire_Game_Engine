#pragma once
#include "Animation.serializer.h"
#include "Avatar.serializer.h"
#include "GameObject.serializer.h"
#include "TextureInfo.serializer.h"
#include "Runtime/IO/AliceResourceType.h"
#include "Runtime/Animation/AnimationClipType.h"
Serializer::GameObject* GetRightSibling(Serializer::GameObject*node);
Serializer::GameObject* GetChild(Serializer::GameObject*parent);
void SetVector3(Serializer::Vector3f*v, float x, float y, float z);
void SetVector4(Serializer::Vector4f*v, float x, float y, float z, float w = 0.0f);
void ExportAssetBundle(const char *path,const char * guid,Alice::ResourceType type,int compressed, const std::string &data);