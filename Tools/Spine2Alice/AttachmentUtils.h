#pragma once
#include <stdio.h>
#include<iostream>
#include <unordered_map>
using namespace std;
#include "Animation.serializer.h"
#include "Avatar.serializer.h"
#include "GameObject.serializer.h"
#include "TextureInfo.serializer.h"
extern "C" {
#include "spine/spine.h"
#include "spine/extension.h"
}
void InitSkinData(const char * path ,const char * skin_name, Serializer::Skin*skin);