#include "Runtime/Scene/EventDispatcher.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Render/Camera.h"
namespace Alice{
	static EventDispatcher sEventDispatcher;
	EventDispatcher::EventDispatcher(){
		mLastTouchObject = nullptr;
		mLastGameObjectHovered = nullptr;
	}
	void EventDispatcher::OnLeftButtonDown(void*param){
		Alice::TouchEvent *te = (Alice::TouchEvent*)param;
		Alice::GetEventDispatcher()->OnTouchBegin(te);
		delete te;
	}
	void EventDispatcher::OnMouseMove(void*param){
		Alice::TouchEvent *te = (Alice::TouchEvent*)param;
		Alice::GetEventDispatcher()->OnTouchMove(te);
		delete te;
	}
	void EventDispatcher::OnLeftButtonUp(void*param){
		Alice::TouchEvent *te = (Alice::TouchEvent*)param;
		Alice::GetEventDispatcher()->OnTouchEnd(te);
		delete te;
	}
	void EventDispatcher::OnRightButtonDown(void*param){
		Alice::TouchEvent *te = (Alice::TouchEvent*)param;
		te->mTouchID += 100;
		Alice::GetEventDispatcher()->OnTouchBegin(te);
		delete te;
	}
	void EventDispatcher::OnMouseMiddleButtonScroll(void*param){
		Alice::MouseWheelEvent *te = (Alice::MouseWheelEvent*)param;
		Alice::GetEventDispatcher()->OnMouseWheel(te);
		delete te;
	}
	void EventDispatcher::OnRightButtonUp(void*param){
		Alice::TouchEvent *te = (Alice::TouchEvent*)param;
		te->mTouchID += 100;
		Alice::GetEventDispatcher()->OnTouchEnd(te);
		delete te;
	}
	void EventDispatcher::OnPaste(const char*param, int len){
		auto iter = mPasteEventListener.begin();
		auto iterEnd = mPasteEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnPaste(param,len);
		}
	}

	void EventDispatcher::AddEventListener(GameObject*go){
		if (mEventListeners.find(go)==mEventListeners.end()){
			mEventListeners.insert(go);
		}
	}

	void EventDispatcher::RemoveEventListener(GameObject*go){
		std::set<GameObject*>::iterator iter = mEventListeners.find(go);
		if (iter!=mEventListeners.end()){
			mEventListeners.erase(iter);
		}
	}

    void EventDispatcher::AddIAPListener(GameObject*go){
        if (mIAPEventListener.find(go)==mIAPEventListener.end()){
            mIAPEventListener.insert(go);
        }
    }
    
    void EventDispatcher::RemoveIAPListener(GameObject*go){
        std::set<GameObject*>::iterator iter = mIAPEventListener.find(go);
        if (iter!=mIAPEventListener.end()){
            mIAPEventListener.erase(iter);
        }
    }

	void EventDispatcher::AddAdRewardListener(GameObject*go) {
		if (mAdRewardEventListener.find(go) == mAdRewardEventListener.end()) {
			mAdRewardEventListener.insert(go);
		}
	}

	void EventDispatcher::RemoveAddAdRewardListener(GameObject*go) {
		std::set<GameObject*>::iterator iter = mAdRewardEventListener.find(go);
		if (iter != mAdRewardEventListener.end()) {
			mAdRewardEventListener.erase(iter);
		}
	}

	void EventDispatcher::AddRightButtonEventListener(GameObject*go){
		if (mRightButtonEventListener.find(go) == mRightButtonEventListener.end()){
			mRightButtonEventListener.insert(go);
		}
	}

	void EventDispatcher::RemoveRightButtonEventListener(GameObject*go){
		std::set<GameObject*>::iterator iter = mRightButtonEventListener.find(go);
		if (iter != mRightButtonEventListener.end()){
			mRightButtonEventListener.erase(iter);
		}
	}

	void EventDispatcher::AddMouseMiddleButtonEventListener(GameObject*go) {
		if (mMouseMiddleButtonEventListener.find(go) == mMouseMiddleButtonEventListener.end()) {
			mMouseMiddleButtonEventListener.insert(go);
		}
	}

	void EventDispatcher::RemoveMouseMiddleButtonEventListener(GameObject*go) {
		std::set<GameObject*>::iterator iter = mMouseMiddleButtonEventListener.find(go);
		if (iter != mMouseMiddleButtonEventListener.end()) {
			mMouseMiddleButtonEventListener.erase(iter);
		}
	}

	void EventDispatcher::AddPasteEventListener(GameObject*go) {
		if (mPasteEventListener.find(go) == mPasteEventListener.end()) {
			mPasteEventListener.insert(go);
		}
	}

	void EventDispatcher::RemovePasteEventListener(GameObject*go) {
		std::set<GameObject*>::iterator iter = mPasteEventListener.find(go);
		if (iter != mPasteEventListener.end()) {
			mPasteEventListener.erase(iter);
		}
	}

	void EventDispatcher::AddKeyboardListener(GameObject*go)
	{
		if (mKeyboardEventListener.find(go) == mKeyboardEventListener.end())
		{
			mKeyboardEventListener.insert(go);
		}
	}

	void EventDispatcher::RemoveKeyboardListener(GameObject*go)
	{
		std::set<GameObject*>::iterator iter = mKeyboardEventListener.find(go);
		if (iter != mKeyboardEventListener.end())
		{
			mKeyboardEventListener.erase(iter);
		}
	}

	void EventDispatcher::CaptureTouchPos(GameObject *go){
		if (mTouchPosListeners.find(go) == mTouchPosListeners.end()){
			mTouchPosListeners.insert(go);
		}
	}

	void EventDispatcher::ReleaseTouchPos(GameObject*go){
		std::set<GameObject*>::iterator iter = mTouchPosListeners.find(go);
		if (iter != mTouchPosListeners.end()){
			mTouchPosListeners.erase(iter);
		}
	}

	void EventDispatcher::OnTouchBegin(TouchEvent *teInfo){
		std::sort(GetSceneManager()->mCameras.begin(), GetSceneManager()->mCameras.end(), Camera::SortCameras);
		for (auto iter = GetSceneManager()->mCameras.rbegin(); iter != GetSceneManager()->mCameras.rend(); iter++){
			//更新所有节点的组件状态
			Camera*currentCamera = *iter;
			if (currentCamera->mCameraMode==Camera::OrthoMode&&currentCamera->IsEnabled()){
				TouchEvent te(teInfo->mX, teInfo->mY);
				te.mTouchID = teInfo->mTouchID;
				currentCamera->ScreenCoordToOpenGLCoord(te.mX, te.mY);
				GameObject*go = nullptr;
				if (GetSceneManager()->mRootObject->mRightSibling != nullptr) {
					go = GetSceneManager()->mRootObject->RightSibling<GameObject>()->Insetersect(&te);
				}
				else {
					go = GetSceneManager()->mRootObject->Insetersect(&te);
				}
				if (go!=nullptr){
					go->OnTouchBegin(te);
					mLastTouchObject = go;
					return;
				}
			}
		}
	}
	void EventDispatcher::OnTouchMove(TouchEvent *teInfo){
		std::sort(GetSceneManager()->mCameras.begin(), GetSceneManager()->mCameras.end(), Camera::SortCameras);
		for (auto iter = GetSceneManager()->mCameras.rbegin(); iter != GetSceneManager()->mCameras.rend(); iter++) {
			//更新所有节点的组件状态
			Camera*currentCamera = *iter;
			if (currentCamera->mCameraMode == Camera::OrthoMode&&currentCamera->IsEnabled()) {
				TouchEvent te(teInfo->mX, teInfo->mY);
				te.mTouchID = teInfo->mTouchID;
				currentCamera->ScreenCoordToOpenGLCoord(te.mX, te.mY);
				GameObject*go = nullptr;
				if (GetSceneManager()->mRootObject->mRightSibling != nullptr) {
					go = GetSceneManager()->mRootObject->RightSibling<GameObject>()->Insetersect(&te);
				}
				else {
					go = GetSceneManager()->mRootObject->Insetersect(&te);
				}
				if (mLastGameObjectHovered != nullptr && go != mLastGameObjectHovered) {
					mLastGameObjectHovered->OnTouchLeave(te);
				}
				if (go != nullptr&&go != mLastGameObjectHovered) {
					go->OnTouchEnter(te);
				}
				mLastGameObjectHovered = go;
				if (mLastGameObjectHovered != nullptr) {
					mLastGameObjectHovered->OnTouchMove(te);
				}
				for (std::set<GameObject*>::iterator iter = mTouchPosListeners.begin(); iter != mTouchPosListeners.end(); iter++) {
					if (mLastGameObjectHovered != (*iter) && (*iter)->IsEnabled())
						(*iter)->OnTouchMove(te);
				}
			}
		}
	}
	void EventDispatcher::OnTouchEnd(TouchEvent *teInfo){
		std::sort(GetSceneManager()->mCameras.begin(), GetSceneManager()->mCameras.end(), Camera::SortCameras);
		for (auto iter = GetSceneManager()->mCameras.rbegin(); iter != GetSceneManager()->mCameras.rend(); iter++){
			//更新所有节点的组件状态
			Camera*currentCamera = *iter;
			if (currentCamera->mCameraMode == Camera::OrthoMode&&currentCamera->IsEnabled()){
				//坐标转换成为当前摄像机下的2D屏幕
				TouchEvent te(teInfo->mX, teInfo->mY);
				te.mTouchID = teInfo->mTouchID;
				currentCamera->ScreenCoordToOpenGLCoord(te.mX, te.mY);
				GameObject*go = nullptr;
				if (GetSceneManager()->mRootObject->mRightSibling != nullptr) {
					go = GetSceneManager()->mRootObject->RightSibling<GameObject>()->Insetersect(&te);
				}
				else {
					go = GetSceneManager()->mRootObject->Insetersect(&te);
				}
				if (mLastTouchObject != nullptr && go != mLastTouchObject) {
					mLastTouchObject->OnTouchCanceled(te);
				}
				if (go != nullptr){
					go->OnTouchEnd(te);
				}
				mLastTouchObject = nullptr;
				if (go!=nullptr){
					return ;
				}
			}
		}
	}
	void EventDispatcher::OnTouchCanceled(TouchEvent *teInfo){
		for (std::set<GameObject*>::iterator iter = mEventListeners.begin(); iter != mEventListeners.end(); iter++){
			(*iter)->OnTouchCanceled(*teInfo);
		}
	}

	void EventDispatcher::OnMouseWheel(MouseWheelEvent *event) {
		for (std::set<GameObject*>::iterator iter = mMouseMiddleButtonEventListener.begin(); iter != mMouseMiddleButtonEventListener.end(); iter++){
			(*iter)->OnMouseWheel(event);
		}
	}

	void EventDispatcher::OnKeyDown(int code){
		auto iter = mKeyboardEventListener.begin();
		auto iterEnd = mKeyboardEventListener.end();
		for (; iter != iterEnd; ++iter){
			(*iter)->OnKeyDown(code);
		}
	}

	void EventDispatcher::OnKeyUp(int code){
		auto iter = mKeyboardEventListener.begin();
		auto iterEnd = mKeyboardEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnKeyUp(code);
		}
	}

	void EventDispatcher::OnIMEChar(const char*utf8Str){
		auto iter = mKeyboardEventListener.begin();
		auto iterEnd = mKeyboardEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnIMEChar(utf8Str);
		}
	}

	void EventDispatcher::OnIMECompositionString(const char*utf8Str, bool isResult) {
		auto iter = mKeyboardEventListener.begin();
		auto iterEnd = mKeyboardEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnIMECompositionString(utf8Str,isResult);
		}
	}
    
    void EventDispatcher::OnEditEnd(){
        auto iter = mKeyboardEventListener.begin();
        auto iterEnd = mKeyboardEventListener.end();
        for (; iter != iterEnd; ++iter) {
            (*iter)->OnEditEnd();
        }
    }

	void EventDispatcher::OnChar(AliceUInt32 charCode){
		auto iter = mKeyboardEventListener.begin();
		auto iterEnd = mKeyboardEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnChar(charCode);
		}
	}
    void EventDispatcher::OnIAPCallback(void *product){
        auto iter = mIAPEventListener.begin();
        auto iterEnd = mIAPEventListener.end();
        for (; iter != iterEnd; ++iter) {
            (*iter)->OnIAP(product);
        }
    }
	void EventDispatcher::OnAdReward(void *product) {
		auto iter = mAdRewardEventListener.begin();
		auto iterEnd = mAdRewardEventListener.end();
		for (; iter != iterEnd; ++iter) {
			(*iter)->OnAdReward(product);
		}
	}
	//coordinate in full resolution mode,need to be scaled to view coordinate
	void OnHandheldTouchBegin(void*param){
		TouchEvent *te = (TouchEvent*)param;
		sEventDispatcher.OnTouchBegin(te);
		delete te;
	}
	void OnHandheldTouchMove(void*param){
		TouchEvent *te = (TouchEvent*)param;
		sEventDispatcher.OnTouchMove(te);
		delete te;
	}
	void OnHandheldTouchEnd(void*param){
		TouchEvent *te = (TouchEvent*)param;
		sEventDispatcher.OnTouchEnd(te);
		delete te;
	}
	void OnHandheldTouchCancel(void*param){
		TouchEvent *te = (TouchEvent*)param;
		sEventDispatcher.OnTouchCanceled(te);
		delete te;
	}
	void OnEditEndRunOnMainThread(void * param) {
		Alice::GetEventDispatcher()->OnEditEnd();
	}
	void OnIMECompositionStringRunOnMainThread(void * param) {
		char * utf8Str = (char*)param;
		Alice::GetEventDispatcher()->OnIMECompositionString(utf8Str,true);
		delete[] utf8Str;
	}
	void OnIMECharRunOnMainThread(void * param) {
		char * utf8Str = (char*)param;
		int len = strlen(utf8Str);
		//Info("OnIMECharRunOnMainThread %s", utf8Str);
		if (len > 0)
		{
			int currentSrcPos = 0;
			int i = 0;
			char szBuffer[8] = { 0 };
			AliceUInt32 temp;
			for (; currentSrcPos < len;)
			{
				int currentLen = Alice::StringUtils::UTF82UTF32((unsigned char*)utf8Str + currentSrcPos, temp);
				if (currentLen == 0)
				{
					delete[] utf8Str;
					return;
				}
				//Info("current char %s len %d", szBuffer, currentLen);
				memset(szBuffer, 0, 8);
				memcpy(szBuffer, utf8Str + currentSrcPos, currentLen);
				if (currentLen == 1) {
					Alice::GetEventDispatcher()->OnChar(szBuffer[0]);
				}
				else {
					Alice::GetEventDispatcher()->OnIMEChar(szBuffer);
				}
				currentSrcPos += currentLen;
			}
		}
		delete[] utf8Str;
	}
	void OnIAP(void * product) {
		Alice::GetEventDispatcher()->OnIAPCallback(product);
		Alice::IAPData*ptr = (Alice::IAPData*)product;
		delete ptr;
	}
	void OnAdReward(void * data) {
		Alice::GetEventDispatcher()->OnAdReward(data);
		Alice::AdReward*ptr = (Alice::AdReward*)data;
		delete ptr;
	}
	EventDispatcher*GetEventDispatcher()
	{
		return &sEventDispatcher;
	}
}
#if ALICE_ANDROID

#elif ALICE_IPHONE
extern "C" void OnTouchBegin(AliceUInt64 touchID,float x, float y)
{
	Alice::TouchEvent*te = new (kMemDefaultId)Alice::TouchEvent;
	te->mX = x;
	te->mY = y;
	te->mTouchID = touchID;
	Alice::MainThreadScheduler::RunInMainThread(Alice::OnHandheldTouchBegin, te);
}

extern "C" void OnTouchEnd(AliceUInt64 touchID, float x, float y)
{
	Alice::TouchEvent*te = new (kMemDefaultId)Alice::TouchEvent;
	te->mX = x;
	te->mY = y;
	te->mTouchID = touchID;
	Alice::MainThreadScheduler::RunInMainThread(Alice::OnHandheldTouchEnd, te);
}

extern "C" void OnTouchMove(AliceUInt64 touchID, float x, float y)
{
	Alice::TouchEvent*te = new (kMemDefaultId)Alice::TouchEvent;
	te->mX = x;
	te->mY = y;
	te->mTouchID = touchID;
	Alice::MainThreadScheduler::RunInMainThread(Alice::OnHandheldTouchMove, te);
}

extern "C" void OnTouchCancel(AliceUInt64 touchID, float x, float y)
{
	Alice::TouchEvent*te = new (kMemDefaultId)Alice::TouchEvent;
	te->mX = x;
	te->mY = y;
	te->mTouchID = touchID;
	Alice::MainThreadScheduler::RunInMainThread(Alice::OnHandheldTouchCancel, te);
}
extern "C" void OnEditEnd(){
    Alice::GetEventDispatcher()->OnEditEnd();
}
extern "C" void OnIMEChar(const char*utf8Str)
{
    int len=strlen(utf8Str);
	if (len > 0)
	{
        int currentSrcPos = 0;
        int i = 0;
        char szBuffer[8]={0};
        AliceUInt32 temp;
        for (; currentSrcPos < len;)
        {
            int currentLen = Alice::StringUtils::UTF82UTF32((unsigned char*)utf8Str + currentSrcPos, temp);
            if (currentLen==0)
            {
                return ;
            }
            memset(szBuffer,0,8);
            memcpy(szBuffer, utf8Str+currentSrcPos, currentLen);
            if(currentLen==1){
                Alice::GetEventDispatcher()->OnChar(szBuffer[0]);
            }else{
                Alice::GetEventDispatcher()->OnIMEChar(szBuffer);
            }
            //Info("current char %s len %d",szBuffer,currentLen);
            currentSrcPos += currentLen;
        }
	}
}
#endif

#if ALICE_OSX_PLAYER || ALICE_IPHONE
extern "C" void OnIAPCallback(void * product){
	Alice::MainThreadMutex::Lock();
    Alice::MainThreadScheduler::RunInMainThread(Alice::OnIAP, product);
	Alice::MainThreadMutex::Unlock();
}
#endif
