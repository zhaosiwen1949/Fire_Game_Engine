#include "ScriptObject.h"
#include "Runtime/LuaEngine/LuaEngine.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Scene/EventDispatcher.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/ResourceManager.h"

namespace Alice
{
	ScriptObject::ScriptObject() :
		mUpdate(LUA_REFNIL),
		mFixedUpdate(LUA_REFNIL),
		mOnChar(LUA_REFNIL),
		mOnIMEChar(LUA_REFNIL),
		mTouchBegin(LUA_REFNIL),
		mTouchEnd(LUA_REFNIL),
		mTouchMove(LUA_REFNIL),
		mTouchCanceled(LUA_REFNIL),
		mMouseWheel(LUA_REFNIL),
		mOnTouchEnter(LUA_REFNIL),
		mOnTouchLeave(LUA_REFNIL),
		mKeyUp(LUA_REFNIL),
		mKeyDown(LUA_REFNIL),
		mAnimationEnd(LUA_REFNIL),
		mPostRendering(LUA_REFNIL)
	{
	}

	void ScriptObject::OnInitScript() {
		lua_State*L = LuaEngine::Store();
		REF_FIELD(L, -2, mUpdate, "Update");
		REF_FIELD(L, -2, mFixedUpdate, "FixedUpdate");
		REF_FIELD(L, -2, mOnChar, "OnChar");
		REF_FIELD(L, -2, mOnIMEChar, "OnIMEChar");
		REF_FIELD(L, -2, mTouchBegin, "OnTouchBegin");
		REF_FIELD(L, -2, mTouchEnd, "OnTouchEnd");
		REF_FIELD(L, -2, mTouchMove, "OnTouchMove");
		REF_FIELD(L, -2, mTouchCanceled, "OnTouchCanceled");
		REF_FIELD(L, -2, mMouseWheel, "OnMouseWheel");
		REF_FIELD(L, -2, mOnTouchEnter, "OnTouchEnter");
		REF_FIELD(L, -2, mOnTouchLeave, "OnTouchLeave");
		REF_FIELD(L, -2, mKeyUp, "OnKeyUp");
		REF_FIELD(L, -2, mKeyDown, "OnKeyDown");
		REF_FIELD(L, -2, mPostRendering, "OnPostRendering");
		LuaEngine::Restore();
	}

	void ScriptObject::OnDestroyScript() {
		lua_State*L = LuaEngine::Store();
		UNREF(L, mUpdate);
		UNREF(L, mFixedUpdate);
		UNREF(L, mOnChar);
		UNREF(L, mOnIMEChar);
		UNREF(L, mTouchBegin);
		UNREF(L, mTouchEnd);
		UNREF(L, mTouchMove);
		UNREF(L, mTouchCanceled);
		UNREF(L, mOnTouchEnter);
		UNREF(L, mOnTouchLeave);
		UNREF(L, mMouseWheel);
		UNREF(L, mKeyUp);
		UNREF(L, mKeyDown);
		UNREF(L, mAnimationEnd);
		UNREF(L, mPostRendering);
		LuaEngine::Restore();
	}

	ScriptObject::~ScriptObject(){
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "ScriptObject");
		lua_pushnil(L);
		lua_setmetatable(L, -2);
		lua_pop(L, 1);
		lua_pushnil(L);
		lua_setfield(L, -2, "ScriptObject");
		lua_pushnil(L);
		lua_setfield(L, -2, "GameObject");
		lua_unref(L, mLuaData);
		mLuaData = LUA_REFNIL;
		//Debug("delete script %s",mName.mText);
		LuaEngine::Restore();
		OnDestroyScript();
	}

	
	void ScriptObject::CleanUpScripts(lua_State*L){
		LuaEngine::Store();
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_pushnil(L);
		lua_setfield(L, -2, "Assets");
		LuaEngine::Restore();
	}

	int ScriptObject::ALICE_NEW_ScriptObject(lua_State*L){
		//input is a script table instance 
		//output is script object
		if (!lua_istable(L,-1)){
			Error("ScriptObject New : arg at -1 is not a lua table %d type %d",lua_gettop(L),lua_type(L,-1));
			lua_pushnil(L);
			return 1;
		}
		//store lua table in c++ data struct
		ScriptObject**pp = (ScriptObject**)lua_newuserdata(L, sizeof(ScriptObject*));
		ScriptObject*pMemory = new (kMemDefaultId)ScriptObject;
		*pp = pMemory;
		SET_LUA_TYPE(L, ScriptObject);
		lua_setfield(L, -2, "ScriptObject");//set script object pointer to input script table field "ScriptObject"
		pMemory->mLuaData = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_getref(L, pMemory->mLuaData);
		pMemory->OnInitScript();
		return 1;
	}

	void ScriptObject::Awake(lua_State*L) {
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(mLuaData, Awake);
	}
	void ScriptObject::Update(float deltaTime) {
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(mLuaData, mUpdate, deltaTime);
	}

	void ScriptObject::OnDestroy() {
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(mLuaData, OnDestroy);
	}

	void ScriptObject::OnShow() {
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(mLuaData, OnShow);
	}

	void ScriptObject::OnHide() {
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(mLuaData, OnHide);
	}

	void ScriptObject::FixedUpdate(float deltaTime) {
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(mLuaData, mFixedUpdate,deltaTime);
	}

	bool ScriptObject::GetMethod(lua_State *L,const char*methodName){
		if (mLuaData != LUA_REFNIL){
			lua_getref(L, mLuaData);
			lua_getfield(L, -1, methodName);
			if (!lua_isnil(L, -1)){
				lua_remove(L, -2);
				return true;
			}else{
				lua_pop(L,2);
				return false;
			}
		}
		return false;
	}

	void ScriptObject::OnTouchBegin(const TouchEvent&te){
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData,mTouchBegin,te.mX, te.mY, te.mTouchID);
	}

	void ScriptObject::OnTouchEnd(const TouchEvent&te) {
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData, mTouchEnd, te.mX, te.mY, te.mTouchID);
	}

	void ScriptObject::OnTouchCanceled(const TouchEvent&te) {
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData, mTouchCanceled, te.mX, te.mY, te.mTouchID);
	}

	void ScriptObject::OnTouchMove(const TouchEvent&te){
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData, mTouchMove, te.mX, te.mY, te.mTouchID);
	}
	void ScriptObject::OnTouchEnter(const TouchEvent&te) {
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData, mOnTouchEnter, te.mX, te.mY, te.mTouchID);
	}
	void ScriptObject::OnTouchLeave(const TouchEvent&te) {
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(mLuaData, mOnTouchLeave , te.mX, te.mY, te.mTouchID);
	}
	void ScriptObject::OnMouseWheel(const MouseWheelEvent*event) {
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_2I(mLuaData, mMouseWheel, event->mKey, event->mWheelDelta);
	}

	void ScriptObject::OnCollideBegin(GameObject*other){
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnCollideBegin");
		if (!lua_isnil(L, -1)){
			lua_pushvalue(L, -2);
			other->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0){
				Error("ScriptObject::OnCollideBegin %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

	void ScriptObject::OnCollideEnd(GameObject*other){
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnCollideEnd");
		if (!lua_isnil(L, -1)){
			lua_pushvalue(L, -2);
			other->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0){
				Error("ScriptObject::OnCollideEnd %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

	void ScriptObject::OnViewportChanged(float width,float height){
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG2F(mLuaData, OnViewportChanged, width, height);
	}

	void ScriptObject::OnAnimationEnd(const char*animationName){
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnAnimationEnd");
		if (!lua_isnil(L, -1)){
			lua_pushvalue(L, -2);
			lua_pushstring(L,animationName);
			if (lua_pcall(L, 2, 0, 0) != 0){
				Error("ScriptObject::OnAnimationEnd %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

	void ScriptObject::OnKeyDown(int code) {
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(mLuaData, mKeyDown, code);
	}

	void ScriptObject::OnKeyUp(int code){
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(mLuaData, mKeyUp, code);
	}

	void ScriptObject::OnIMEChar(const char*utf8Str) {
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnIMEChar");
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, -2);
			lua_pushstring(L, utf8Str);
			if (lua_pcall(L, 2, 0, 0) != 0) {
				Error("ScriptObject::OnIMEChar %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}
	void ScriptObject::OnIMECompositionString(const char*utf8Str, bool isResult) {
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnIMECompositionString");
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, -2);
			lua_pushstring(L, utf8Str);
			lua_pushboolean(L, isResult);
			if (lua_pcall(L, 3, 0,-6) != 0) {
				Error("ScriptObject::OnIMECompositionString %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

	void ScriptObject::OnEditEnd() {
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(mLuaData, OnEditEnd);
	}

	void ScriptObject::OnChar(AliceUInt32 charCode) {
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(mLuaData, mOnChar, charCode);
	}

	void ScriptObject::OnPaste(const char*str, int len) {
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnPaste");
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, -2);
			lua_pushlstring(L, str,len);
			if (lua_pcall(L, 2, 0, 0) != 0) {
				Error("ScriptObject::OnPaste %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

    void ScriptObject::OnIAP(void*product) {
        lua_State*L = LuaEngine::Store();
        lua_getref(L, mLuaData);
        lua_getfield(L, -1, "OnIAP");
        if (lua_isfunction(L, -1)) {
            lua_pushvalue(L, -2);
            IAPData *iapData=(IAPData*)product;
            lua_pushlstring(L,iapData->mProduct.mText,iapData->mProduct.mLen);
            lua_pushlstring(L,iapData->mBill.mText,iapData->mBill.mLen);
            if (lua_pcall(L, 3, 0, 0) != 0) {
                Error("ScriptObject::OnIAP %s", lua_tostring(L, -1));
            }
        }
        LuaEngine::Restore();
	}
	void ScriptObject::OnAdReward(void*data) {
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, "OnAdReward");
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, -2);
			AdReward *reward = (AdReward*)data;
			lua_pushlstring(L, reward->mName.mText, reward->mName.mLen);
			lua_pushinteger(L, reward->mCount);
			//Info("OnAdReward %s,%d",reward->mName.mText,reward->mCount);
			if (lua_pcall(L, 3, 0, 0) != 0) {
				Error("ScriptObject::OnAdReward %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}
	void ScriptObject::OnPostRendering(Object*colorBuffer) {
		if (mPostRendering != LUA_REFNIL) {
			lua_State *L = Alice::LuaEngine::Store(); 
			lua_getref(L, mPostRendering);
			lua_getref(L, mLuaData); 
			colorBuffer->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0) {
				Error("ScriptObject::OnPostRendering : %s",lua_tostring(L, -1)); 
			}
			Alice::LuaEngine::Restore(); 
		}
	}

	int ScriptObject::Export(lua_State*L){
		luaL_Reg apim[] = {
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, ScriptObject, apim);
		SET_PARENT(L,Component);
		return 0;
	}

	int ScriptObject::DefaultGC(lua_State*L) {
		Debug("ScriptObject::DefaultGC");
		return 0;
	}

	void ScriptObject::Call(const char *method, Param *param) {
		lua_State*L = LuaEngine::Store();
		lua_getref(L, mLuaData);
		lua_getfield(L, -1, method);
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, -2);
			Param*arg = param;
			int argc = 1;
			while (arg !=nullptr){
				arg->PushValue(L);
				arg = arg->Next<Param>();
				argc++;
			}
			if (lua_pcall(L, argc, 0, -2 - argc) != 0) {
				Error("ScriptObject::Call %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}
}
