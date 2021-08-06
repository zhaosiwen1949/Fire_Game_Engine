#pragma once
#include "GameObject.h"
#include <set>

namespace Alice{
    struct IAPData{
        FixedString mProduct;
        FixedString mBill;
	};
	struct AdReward {
		FixedString mName;
		int mCount;
	};
	class EventDispatcher
	{
	public:
		EventDispatcher();
		void AddEventListener(GameObject*go);
		void RemoveEventListener(GameObject*go);
		void AddRightButtonEventListener(GameObject*go);
		void RemoveRightButtonEventListener(GameObject*go);
		void AddMouseMiddleButtonEventListener(GameObject*go);
		void RemoveMouseMiddleButtonEventListener(GameObject*go);
		void AddPasteEventListener(GameObject*go);
		void RemovePasteEventListener(GameObject*go);
		void AddKeyboardListener(GameObject*go);
		void RemoveKeyboardListener(GameObject*go);
        void AddIAPListener(GameObject*go);
		void RemoveIAPListener(GameObject*go);
		void AddAdRewardListener(GameObject*go);
		void RemoveAddAdRewardListener(GameObject*go);

		void CaptureTouchPos(GameObject*go);
		void ReleaseTouchPos(GameObject*go);

		void OnTouchBegin(TouchEvent *te);
		void OnTouchEnd(TouchEvent *te);
		void OnTouchMove(TouchEvent *te);
		void OnTouchCanceled(TouchEvent *te);
		void OnMouseWheel(MouseWheelEvent *event);
		//keyboard
		void OnKeyDown(int code);
		void OnKeyUp(int code);
        void OnEditEnd();
		void OnIMEChar(const char*utf8Str);
		void OnIMECompositionString(const char*utf8Str,bool isResult);
		void OnChar(AliceUInt32 charCode);
		std::set<GameObject*> mEventListeners;//left mouse button or the first finger that touch screen
		std::set<GameObject*> mRightButtonEventListener;
		std::set<GameObject*> mMouseMiddleButtonEventListener;
		std::set<GameObject*> mPasteEventListener;
        std::set<GameObject*> mIAPEventListener;
		std::set<GameObject*> mKeyboardEventListener;
		std::set<GameObject*> mAdRewardEventListener;
		std::set<GameObject*> mTouchPosListeners;
		GameObject*mLastTouchObject,*mLastGameObjectHovered;
	public:
		static void OnLeftButtonDown(void*param);
		static void OnLeftButtonUp(void*param);
		static void OnRightButtonDown(void*param);
		static void OnMouseMiddleButtonScroll(void*param);
		static void OnRightButtonUp(void*param);
		void OnIAPCallback(void * product);
		void OnAdReward(void * product);
		void OnPaste(const char*param,int len);
		static void OnMouseMove(void*param);
	};
	void OnHandheldTouchBegin(void*param);
	void OnHandheldTouchMove(void*param);
	void OnHandheldTouchEnd(void*param);
	void OnHandheldTouchCancel(void*param);
	void OnEditEndRunOnMainThread(void * param);
	void OnIMECharRunOnMainThread(void * param);
	void OnIMECompositionStringRunOnMainThread(void * param);
	void OnIAP(void * product);
	void OnAdReward(void * product);
	EventDispatcher*GetEventDispatcher();
}
