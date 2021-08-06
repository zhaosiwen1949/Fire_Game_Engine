#include "TouchEvent.h"

namespace Alice
{
	TouchEvent::TouchEvent():mTouchID(0),mX(-10000.0f),mY(-10000.0f){
		
	}

	TouchEvent::TouchEvent(float x, float y) : mTouchID(0), mX(x), mY(y) {

	}

	IMECharEvent::IMECharEvent() : mCharCode(32){

	}
	AdInfo::AdInfo() {
		mRewardVideoStatus = 0;
		mInterstitialStatus = 0;
		mBannerStatus = 0;
	}
}