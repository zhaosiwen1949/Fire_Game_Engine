#pragma once
#ifdef ALICE_PLATFORM_WIN
#include "Win.h"
#elif ALICE_OSX_PLAYER
#include "Mac.h"
#elif ALICE_IPHONE
#include "iPhone.h"
#include "iOSAPIBridge.h"
#elif ALICE_ANDROID
#include "Android.h"
#endif
