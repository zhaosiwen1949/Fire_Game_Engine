#pragma once
#include "ggl.h"
unsigned char * LoadFileContent(const char*path,int&filesize);
float GetFrameTime();
GLuint CreateTexture2DFromPNG(const char *imgFilePath, bool invertY=true);