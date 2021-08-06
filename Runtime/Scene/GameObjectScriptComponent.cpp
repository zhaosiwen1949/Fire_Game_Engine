#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/Camera.h"
namespace Alice{
	bool GameObject::AttachScript(const char*scriptName) {
		bool ret = false;
		lua_State*L = LuaEngine::s_GlobalStatePtr;
		//get script from cache
		if (LuaEngine::LoadInternalScript(L, scriptName)) {
			//if success,the top will be the request script table
			if (lua_istable(L, -1)) {
				//if everything is alright,the we create the instance of the script
				lua_newtable(L);//script instance
				PushUserData(L);
				lua_setfield(L, -2, "GameObject");//instance.GameObject=mUserData;and top is now script instance
				lua_pushvalue(L, -2);//clone meta table -> user defined script
				lua_pushvalue(L, -1);//value
				lua_setfield(L, -2, "__index");//metatable.__index=metatable,top is again script instance
				lua_setmetatable(L, -2);//now again script instance : setmetatable(obj,M) now script inherit from the metatable(user defined lua script)
										//top is script instance now;
				Alice::ScriptObject::ALICE_NEW_ScriptObject(L);//return the lua table instance
				if (!lua_istable(L, -1)) {//fail to new script object
					errorC("GameObject:AttachScript,fail to new script object");
					lua_pushnil(L);
				}
				else {
					lua_getfield(L, -1, "ScriptObject");
					ScriptObject*so = TO_USERDATA(L, ScriptObject, -1);
					lua_getfield(L, -2, "Name");
					so->mName = lua_tostring(L, -1);
					so->SetOwner(this,true);
					so->Awake(L);
					lua_pop(L, 1);
					so->PushUserData(L);
					ret = true;
				}
			}
			else {
				errorC("GameObject:AttachScript,script attached to game object must return a table,%s does not retuen a table", lua_tostring(L, 2));
				lua_pushnil(L);
			}
		}
		else {
			errorC("GameObject:AttachScript,LoadScript fail %s", lua_tostring(L, 2));
		}
		return ret;
	}

	bool GameObject::DetachScript(ScriptObject*so){
		ScriptObject*current = mScripts;
		ScriptObject*next = current->Next<ScriptObject>();
		while (current != nullptr) {
			if (current == so) {//find the script to detach
				current->LeaveList();//detach the script
				if (current == mScripts) {//if node to be deleted is head node
					mScripts = next;
				}
				return true;
			}
			current = next;
			next = current->Next<ScriptObject>();
		}
		return false;
	}

	ScriptObject*GameObject::GetScript(const char*scriptName){
		ScriptObject*current = mScripts;
		while (current != nullptr)
		{
			if (current->mName == scriptName)
			{
				return current;
			}
			current = current->Next<ScriptObject>();
		}
		return nullptr;
	}

	void GameObject::OnPaste(const char*str, int len) {
		if (IsEnabled()) {
			ScriptObject*current = mScripts;
			while (current != nullptr) {
				ScriptObject*next = current->Next<ScriptObject>();
				if (current->mbEnable)current->OnPaste(str, len);
				current = next;
			}
		}
	}

	void GameObject::OnIAP(void*product) {
		if (IsEnabled()) {
			ScriptObject*current = mScripts;
			while (current != nullptr) {
				ScriptObject*next = current->Next<ScriptObject>();
				if (current->mbEnable)current->OnIAP(product);
				current = next;
			}
		}
	}

	void GameObject::OnAdReward(void*product) {
		if (IsEnabled()) {
			ScriptObject*current = mScripts;
			while (current != nullptr) {
				ScriptObject*next = current->Next<ScriptObject>();
				if (current->mbEnable)current->OnAdReward(product);
				current = next;
			}
		}
	}
	void GameObject::PostRendering(Camera*camera) {
		if (IsEnabled()) {
			if (camera != nullptr&&camera->mFBO != nullptr&&camera->mFBO->mColorBuffer != nullptr) {
				ScriptObject*current = mScripts;
				while (current != nullptr) {
					ScriptObject*next = current->Next<ScriptObject>();
					if (current->mbEnable)current->OnPostRendering(camera->mFBO->mColorBuffer.mPtr);
					current = next;
				}
			}
		}
	}
	void GameObject::Call(const char *method, Param *param) {
		if (IsEnabled()) {
			ScriptObject*current = mScripts;
			while (current != nullptr) {
				ScriptObject*next = current->Next<ScriptObject>();
				if (current->mbEnable) {
					current->Call(method, param);
				}
				current = next;
			}
		}
	}
}