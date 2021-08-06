#pragma once
#include <stdio.h>
#include<iostream>
#include <unordered_map>
using namespace std;
#include "Animation.serializer.h"
#include "Avatar.serializer.h"
#include "GameObject.serializer.h"
#include "TextureInfo.serializer.h"
#include "External/Glew/include/GL/glew.h"
extern "C" {
#include "spine/spine.h"
#include "spine/extension.h"
#include "../src/spine/Json.h"
}
char* LoadFileContent(const char * path, int & len);
void SaveFile(const char * path, const void * bytes, int len);
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path);
void _spAtlasPage_disposeTexture(spAtlasPage* self);
char* _spUtil_readFile(const char* path, int* length);
float HexToColor(const char* value, int index);
void ReadSpineVertices(Json* attachmentMap, spVertexAttachment* attachment, int verticesLength);