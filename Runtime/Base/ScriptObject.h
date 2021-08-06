#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/LuaEngine/LuaCallback.h"

namespace Alice{
	class GameObject;
	class TouchEvent;
	class MouseWheelEvent;
	class ScriptObject:public Component{
	public:
		DECLEAR_ALICE_CLASS(ScriptObject)
	public:
		int mUpdate;
		int mFixedUpdate;
		int mOnChar;
		int mOnIMEChar;
		int mTouchBegin;
		int mTouchEnd;
		int mTouchCanceled;
		int mTouchMove;
		int mOnTouchEnter;
		int mOnTouchLeave;
		int mMouseWheel;
		int mKeyUp;
		int mKeyDown;
		int mAnimationEnd;
		int mPostRendering;
		ScriptObject();
		virtual ~ScriptObject();
		static int Export(lua_State*L);
		static int DefaultGC(lua_State*L);
		void Awake(lua_State*L);
		void OnInitScript();
		void OnDestroyScript();
		bool GetMethod(lua_State *L, const char*methodName);
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void OnTouchBegin(const TouchEvent&te);
		void OnTouchEnd(const TouchEvent&te);
		void OnTouchCanceled(const TouchEvent&te);
		void OnTouchMove(const TouchEvent&te);
		void OnTouchEnter(const TouchEvent&te);
		void OnTouchLeave(const TouchEvent&te);
		void OnMouseWheel(const MouseWheelEvent*event);
		void OnCollideBegin(GameObject*other);
		void OnCollideEnd(GameObject*other);
		void OnDestroy();
		void OnShow();
		void OnHide();
		void OnViewportChanged(float width, float height);

		void OnKeyUp(int code);
		void OnKeyDown(int code);
		void OnIMEChar(const char*utf8Str);
		void OnIMECompositionString(const char*utf8Str, bool isResult);
		void OnEditEnd();//mobile -> edit end,clear current content of input control
		void OnChar(AliceUInt32 charCode);
		void OnPaste(const char*str, int len);
		void OnIAP(void*product);
		void OnAdReward(void*product);
		void OnAnimationEnd(const char*animationName);
		void OnPostRendering(Object*colorBuffer);
		void Call(const char *method,Param *param);
		static int ALICE_NEW_ScriptObject(lua_State*L);
		static void CleanUpScripts(lua_State*L);
	};
}
