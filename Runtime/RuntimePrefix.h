#pragma once
#define _00000000_00000000_00000000_00000001 1
#define _00000000_00000000_00000000_00000010 2
#define _00000000_00000000_00000000_00000100 4
#define _00000000_00000000_00000000_00001000 8
#define _00000000_00000000_00000000_00010000 16
#define _00000000_00000000_00000000_00100000 32
#define _00000000_00000000_00000000_01000000 64
#define _00000000_00000000_00000000_10000000 128
#define _00000000_00000000_00000001_00000000 256
#define _00000000_00000000_00000010_00000000 512
#define _00000000_00000000_00000100_00000000 1024
#define _00000000_00000000_00001000_00000000 2048
#define _00000000_00000000_00010000_00000000 4096
#define _00000000_00000000_00100000_00000000 8192
#define _00000000_00000000_01000000_00000000 16384
#define _00000000_00000000_10000000_00000000 32768
#define ALICE_DEPRECATED __declspec(deprecated)
typedef signed char AliceSInt8;
typedef unsigned char AliceUInt8;
typedef AliceUInt8 AliceUByte;
typedef AliceSInt8 AliceByte;
typedef signed short AliceSInt16;
typedef unsigned short AliceUInt16;
typedef int AliceSInt32;
typedef unsigned int AliceUInt32;
typedef unsigned long long AliceUInt64;
typedef signed long long AliceSInt64;
typedef AliceSInt32 LuaScriptHandle;
typedef void* AliceAny;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#define ALICE_MAX_PATH_LEN 260
#define RELEASE (!_DEBUG)
#if ALICE_EDITOR || ALICE_WIN_PLAYER
#define ALICE_PLATFORM_WIN 1
#include <WinSock2.h>
#include <windows.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <ShlObj.h>
#include <WinNls.h>
#include <wchar.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include <ObjBase.h>
using namespace Gdiplus;
#define NOMINMAX
#include <time.h>
#ifndef TCHAR
#define WCHAR TCHAR
#endif
#elif ALICE_IPHONE || ALICE_ANDROID || ALICE_OSX_PLAYER
#define ALICE_PLATFORM_UNIX 1
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <net/if.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#ifdef ALICE_IPHONE
#include <ifaddrs.h>
#endif
#ifdef ALICE_ANDROID
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#define  LOG_TAG    "Alice"
#define JAVA_API(return_type) extern "C" JNIEXPORT return_type JNICALL
#define JFOO(CLS,METHOD) Java_cn_battlefire_androidplayer_##CLS##_##METHOD
#endif
typedef int64_t __int64;
#endif
#if ALICE_PLATFORM_WIN
#define ALICE_STRICMP _stricmp 
#else 
#define ALICE_STRICMP strcasecmp
#endif
#if ALICE_EDITOR && _DEBUG
#define OPT(return_type) virtual return_type
#else
#define OPT(return_type) return_type
#endif