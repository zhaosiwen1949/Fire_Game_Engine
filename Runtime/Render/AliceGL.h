#pragma once
#include "Runtime/RuntimePrefix.h"
#if ALICE_EDITOR||ALICE_WIN_PLAYER
#define GLEW_STATIC
#include "External/Glew/include/GL/glew.h"
#include "External/Glew/include/GL/wglew.h"
#elif ALICE_OSX_PLAYER
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#elif ALICE_ANDROID
#define ALICE_ES 1
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#ifndef GL_TEXTURE_EXTERNAL_OES
#define GL_TEXTURE_EXTERNAL_OES                          0x8D65
#endif
#ifndef GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH24_STENCIL8_OES                          0x88F0
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES							     0x8D64
#endif
#elif ALICE_IPHONE
#define ALICE_ES 1
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#ifndef GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH24_STENCIL8_OES                          0x88F0
#endif
#endif
#if ALICE_ES || ALICE_OSX_PLAYER
#define GL_CLAMP GL_CLAMP_TO_EDGE
#endif