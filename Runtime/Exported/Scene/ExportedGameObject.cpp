#include "ExportedGameObject.h"
#include "Runtime/Exported/Render/ExportedCamera.h"
#include "Runtime/Exported/Component/ExportedAudioSource.h"
#include "Runtime/Exported/Component/ExportedImageSprite.h"
#include "Runtime/Exported/Component/ExportedImageSprite9.h"
#include "Runtime/Exported/Component/ExportedLabel.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DComponent.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Exported/Math/ExportedMatrix4x4.h"

namespace Alice
{
	int ExportedGameObject::API_Call(lua_State*L)
	{
		if (IsInvokeValid<GameObject>(L, -2, "Call"))
		{
			//invoke with no arg
			ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, -2);
			const char*fooName = lua_tostring(L, -1);
			ScriptObject*script = ptr->mScripts;
			while (script != nullptr)
			{
				if (script->GetMethod(L, fooName))
				{
					//method script
					script->PushUserData(L);
					if (lua_pcall(L, 1, 0, 0) != 0)
					{
						errorC("game object call method %s %s", fooName, lua_tostring(L, -1));
						lua_pop(L, 1);
					}
				}
				script = script->Next<ScriptObject>();
			}
		}
		else if (IsInvokeValid<GameObject>(L, -3, "Call"))
		{
			//invoke with arg
			ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, -3);
			const char*fooName = lua_tostring(L, -2);
			ScriptObject*script = ptr->mScripts;
			while (script != nullptr)
			{
				if (script->GetMethod(L, fooName))
				{
					//method script
					script->PushUserData(L);
					lua_pushvalue(L, -3);
					if (lua_pcall(L, 2, 0, 0) != 0)
					{
						errorC("game object call method %s %s", fooName, lua_tostring(L, -1));
						lua_pop(L, 1);
					}
				}
				script = script->Next<ScriptObject>();
			}
		}
		else
		{
			errorC("GameObject:Call invalid invoke");
		}
		return 0;
	}

	int ExportedGameObject::API_AttachScript(lua_State*L){
		if (!lua_isstring(L, 2)){
			luaL_error(L, "GameObject:AttachScript %s", LUA_TYPE(L, 2));
			lua_pushnil(L);
			return 1;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		if (false==ptr->AttachScript(lua_tostring(L, 2))) {
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedGameObject::API_DetachScript(lua_State*L)
	{
		//1、Script Instance
		if (!lua_istable(L, -1)){
			errorC("DetachScript the arg at -1 must be the script instance");
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject,-2);
		lua_getfield(L, -1, "ScriptObject");
		ScriptObject*so = TO_USERDATA(L, ScriptObject, -1);
		if (ptr->DetachScript(so)){
			delete so;
		}
		return 0;
	}

	int ExportedGameObject::API_GetScript(lua_State*L)
	{
		//1、GameObject ��2、Script path
		if (!lua_isstring(L, -1))
		{
			errorC("GameObject:GetScript ,arg at index 2 must be string,current is %s", LUA_TYPE(L,-1));
			lua_pushnil(L);
			return 1;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, -2);
		const char* scriptPath = lua_tostring(L, -1);
		ScriptObject*so = ptr->GetScript(scriptPath);
		if (so != nullptr)
		{
			so->PushUserData(L);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}
	static int GetComponentID(lua_State*L,int index) {
		if (lua_istable(L, index)) {
			lua_getfield(L, index, "ClassID");
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 1, 0) == 0) {
					if (lua_isnumber(L, -1)) {
						return lua_tointeger(L,-1);
					}
				}
			}
		}
		return -1;
	}
	int ExportedGameObject::AddComponentWithArg(lua_State*L) {
		int componentID = GetComponentID(L, -2);
		if (componentID != -1) {
			Component*component = AddComponent(componentID,L);
			if (component != nullptr) {
				component->PushUserData(L);
				return 1;
			} else {
				errorC("GameObject:AddComponentWithArg fail!Cannot add this component with component id %d", componentID);
				lua_pushnil(L);
				return 1;
			}
		}
		errorC("GameObject:AddComponentWithArg fail! cannot get component id of %s", LUA_TYPE(L, -2));
		lua_pushnil(L);
		return 1;
	}
	int ExportedGameObject::AddComponentNoArg(lua_State*L) {
		int componentID = GetComponentID(L, -1);
		if (componentID != -1) {
			Component*component = AddComponent(componentID);
			if (component != nullptr) {
				component->PushUserData(L);
				return 1;
			} else {
				errorC("GameObject:AddComponentNoArg fail!Cannot add this component with component id %d",componentID);
				lua_pushnil(L);
				return 1;
			}
		}
		errorC("GameObject:AddComponent fail! cannot get component id of %s", LUA_TYPE(L, -1));
		lua_pushnil(L);
		return 1;
	}
	int ExportedGameObject::API_AddComponent(lua_State*L){
		//1、GameObject ��2、Component ; 3 : external arg
		int argc = lua_gettop(L);
		if (argc==3){
			ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, -3);
			return ptr->AddComponentWithArg(L);
		}else if (argc == 2) {
			ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, -2);
			return ptr->AddComponentNoArg(L);
		}
		errorC("GameObject:AddComponent fail! arg count must be 2 or 3", LUA_TYPE(L, -1));
		lua_pushnil(L);
		return 1;
	}

	int ExportedGameObject::API_GetComponent(lua_State*L)
	{
		if (!lua_istable(L, -1))
		{
			errorC("GetComponent fail! arg at index -1 is not a table %s", LUA_TYPE(L,-1));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject,-2);
		lua_getfield(L, -1, "ClassID");
		lua_pcall(L, 0, 1, 0);
		int classID = lua_tointeger(L, -1);
		Component*component = ptr->GetComponent(classID);
		if (component != nullptr)
		{
			component->PushUserData(L);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedGameObject::API_RemoveComponent(lua_State*L)
	{
		if (!IsInvokeValid<GameObject>(L, -2, "RemoveComponent"))
		{
			errorC("GameObject:RemoveComponent invalid invoke");
			return 0;
		}
		if (!lua_isuserdata(L, -1))
		{
			errorC("GameObject::API_RemoveComponent arg at -1 must be some component instance");
			return 0;
		}
		GameObject*ptr = TO_USERDATA(L, GameObject, -2);
		if (Object::IsValid(L, -1)){
			Component*component = TO_USERDATA(L, Component, -1);
			ptr->RemoveComponent(component);
			delete component;
		}
		else
		{
			errorC("component to remove is not valid");
		}
		return 0;
	}

	int ExportedGameObject::API_EnableKeyboardEvent(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		if (lua_gettop(L) == 1) {
			GetEventDispatcher()->AddKeyboardListener(ptr);
		}else{
			if (lua_toboolean(L, 2)) {
				GetEventDispatcher()->AddKeyboardListener(ptr);
			}
			else {
				GetEventDispatcher()->RemoveKeyboardListener(ptr);
			}
		}
		return 0;
	}

	int ExportedGameObject::API_EnableRightMouseEvent(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		if (lua_toboolean(L, 2)) {
			GetEventDispatcher()->AddRightButtonEventListener(ptr);
		} else {
			GetEventDispatcher()->RemoveRightButtonEventListener(ptr);
		}
		return 0;
	}

	int ExportedGameObject::API_EnableMiddleMouseEvent(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		if (lua_toboolean(L, 2)) {
			GetEventDispatcher()->AddMouseMiddleButtonEventListener(ptr);
		}
		else {
			GetEventDispatcher()->RemoveMouseMiddleButtonEventListener(ptr);
		}
		return 0;
	}

	int ExportedGameObject::API_EnablePasteEvent(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		if (lua_toboolean(L, 2)) {
			GetEventDispatcher()->AddPasteEventListener(ptr);
		}
		else {
			GetEventDispatcher()->RemovePasteEventListener(ptr);
		}
		return 0;
	}

    int ExportedGameObject::API_EnableIAPEvent(lua_State*L)
    {
        GameObject*ptr = TO_USERDATA(L, GameObject, 1);
        if (lua_toboolean(L, 2)) {
            GetEventDispatcher()->AddIAPListener(ptr);
        }
        else {
            GetEventDispatcher()->RemoveIAPListener(ptr);
        }
        return 0;
    }

	int ExportedGameObject::API_EnableAdRewardEvent(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		if (lua_toboolean(L, 2)) {
			GetEventDispatcher()->AddAdRewardListener(ptr);
		}
		else {
			GetEventDispatcher()->RemoveAddAdRewardListener(ptr);
		}
		return 0;
	}

	int ExportedGameObject::API_EnableReceiveRay(lua_State*L)
	{
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		ptr->mbReceiveRayCast = lua_toboolean(L, 2);
		return 0;
	}

	int ExportedGameObject::API_AddChild(lua_State*L)
	{
		if (lua_gettop(L) != 2)
		{
			errorC("GameObject:AddChild accept 2 args,current arg count is %d", lua_gettop(L));
			return 0;
		}
		if (!IsInvokeValid<GameObject>(L, 1, "AddChild"))
		{
			errorC("GameObject:AddChild invalid invoke");
			return 0;
		}
		if (!IsType<GameObject>(L,2))
		{
			errorC("GameObject:AddChild arg at index 2 must be game object,current is %s", LUA_TYPE(L,2));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->AddChild(TO_USERDATA(L, ExportedGameObject, 2));
		return 0;
	}

	int ExportedGameObject::API_InsertBefore(lua_State*L)
	{
		if (lua_gettop(L) != 2)
		{
			errorC("GameObject:InsertBefore accept 2 args,current arg count is %d", lua_gettop(L));
			return 0;
		}
		if (!IsInvokeValid<GameObject>(L, 1, "InsertBefore"))
		{
			errorC("GameObject:InsertBefore invalid invoke");
			return 0;
		}
		if (!IsType<GameObject>(L, 2))
		{
			errorC("GameObject:InsertBefore arg at index 2 must be game object,current is %s", LUA_TYPE(L,2));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->InsertBefore(TO_USERDATA(L, ExportedGameObject, 2));
		return 0;
	}

	int ExportedGameObject::API_InsertAfter(lua_State*L)
	{
		if (lua_gettop(L) != 2)
		{
			errorC("GameObject:InsertAfter accept 2 args,current arg count is %d", lua_gettop(L));
			return 0;
		}
		if (!IsInvokeValid<GameObject>(L, 1, "InsertAfter"))
		{
			errorC("GameObject:InsertAfter invalid invoke");
			return 0;
		}
		if (!IsType<GameObject>(L, 2))
		{
			errorC("GameObject:InsertAfter arg at index 2 must be game object,current is %s", LUA_TYPE(L,2));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ExportedGameObject*target = TO_USERDATA(L, ExportedGameObject, 2);
		ptr->InsertAfter(target);
		return 0;
	}

	int ExportedGameObject::API_SetPosition(lua_State*L){
		if (lua_gettop(L) != 4){
			errorC("GameObject:SetPosition arg count must be 3,current is %d", lua_gettop(L) - 1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)){
			errorC("GameObject:SetPosition arg must be number,current 2(%s),3(%s),4(%s)", LUA_TYPE(L,2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);

		ptr->SetLocalPosition(x, y, z);
		return 0;
	}


	int ExportedGameObject::API_SetScale(lua_State*L){
		if (lua_gettop(L) != 4){
			errorC("GameObject:SetScale arg count must be 3,current is %d", lua_gettop(L)-1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)){
			errorC("GameObject:SetScale arg must be number,current 2(%s),3(%s),4(%s)", LUA_TYPE(L,2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->SetLocalScale(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		return 0;
	}
	int ExportedGameObject::API_SetLayer(lua_State*L){
		if (!lua_isnumber(L, 2)){
			errorC("GameObject:SetLayer arg at index 2 must be integer,current arg at index 2 type %s", LUA_TYPE(L, 2));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->mLayer=lua_tointeger(L,2);
		return 0;
	}

	int ExportedGameObject::API_SetName(lua_State*L){
		if (!lua_isstring(L, 2)){
			errorC("GameObject:SetName arg must be string,current arg type %s", LUA_TYPE(L,2));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->SetName(lua_tostring(L, 2));
		return 0;
	}

	int ExportedGameObject::API_GetName(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		lua_pushstring(L, ptr->mName.mText);
		return 1;
	}

	int ExportedGameObject::API_GetRotation(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		lua_pushnumber(L, ptr->mLocalTransform.mRotation.x);
		lua_pushnumber(L, ptr->mLocalTransform.mRotation.y);
		lua_pushnumber(L, ptr->mLocalTransform.mRotation.z);
		return 3;
	}

	int ExportedGameObject::API_GetPosition(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		lua_pushnumber(L, ptr->mLocalTransform.mPosition.x);
		lua_pushnumber(L, ptr->mLocalTransform.mPosition.y);
		lua_pushnumber(L, ptr->mLocalTransform.mPosition.z);
		return 3;
	}

	int ExportedGameObject::API_GetScale(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		lua_pushnumber(L, ptr->mLocalTransform.mScale.x);
		lua_pushnumber(L, ptr->mLocalTransform.mScale.y);
		lua_pushnumber(L, ptr->mLocalTransform.mScale.z);
		return 3;
	}
	int ExportedGameObject::API_SetShearing(lua_State*L) {
		if (lua_gettop(L) != 4) {
			errorC("GameObject:SetShearing arg count must be 3,current is %d", lua_gettop(L) - 1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)) {
			errorC("GameObject:SetShearing arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->mLocalTransform.SetLocalShearing(x, y, z);
		return 0;
	}
	int ExportedGameObject::API_GetShearing(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		lua_pushnumber(L, ptr->mLocalTransform.mShearing.x);
		lua_pushnumber(L, ptr->mLocalTransform.mShearing.y);
		lua_pushnumber(L, ptr->mLocalTransform.mShearing.z);
		return 3;
	}

	int ExportedGameObject::API_SetRotation(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		if (lua_gettop(L) == 2){
			if (!lua_isnumber(L, 2)){
				errorC("GameObject:SetRotation arg must be number,current 1(%s)", LUA_TYPE(L,2));
				return 0;
			}
			ptr->SetLocalRotation(0, 0, lua_tonumber(L, 2));
		}
		else if (lua_gettop(L) == 4)
		{
			if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
			{
				errorC("GameObject:SetRotation arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L,2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
				return 0;
			}
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			ptr->SetLocalRotation(x, y, z);
		}
		return 0;
	}
	int ExportedGameObject::API_GetWorldToModelMatrix(lua_State*L) {
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		ExportedMatrix4x4::New_ExportedAliceType(L);
		ExportedMatrix4x4 * mat4 = TO_USERDATA(L, ExportedMatrix4x4, -1);
		InvertMatrix4x4(ptr->GetWorldMatrix().mData, mat4->mData);
		return 1;
	}
	int ExportedGameObject::API_GetModelMatrix(lua_State*L) {
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		ExportedMatrix4x4::New_ExportedAliceType(L);
		ExportedMatrix4x4 * mat4 = TO_USERDATA(L, ExportedMatrix4x4, -1);
		memcpy(mat4->mData, ptr->GetWorldMatrix().mData, sizeof(float)*16);
		return 1;
	}
	int ExportedGameObject::API_FindChild(lua_State*L){
		if (lua_gettop(L) != 2){
			errorC("GameObject:FindChild arg count must be 2,current arg count is %d", lua_gettop(L));
			lua_pushnil(L);
			return 1;
		}
		if (!lua_isstring(L, 2)){
			errorC("GameObject:FindChild arg at index 2 must be string,current arg is %s", LUA_TYPE(L,2));
			lua_pushnil(L);
			return 1;
		}
		GameObject*ptr = TO_USERDATA(L, GameObject, 1);
		const char*childName = lua_tostring(L, 2);
		GameObject*target = ptr->FindChild(childName);
		if (target != nullptr){
			target->PushUserData(L);
		}else{
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedGameObject::API_Find(lua_State*L){
		if (lua_gettop(L) != 1){
			errorC("ExportedGameObject.Find arg count must be 1 string,current arg count %d", lua_gettop(L));
			lua_pushnil(L);
			return 1;
		}
		if (!lua_isstring(L, 1)){
			errorC("ExportedGameObject.Find arg count must be 1 string,current arg is %s", lua_typename(L, lua_type(L, 1)));
			lua_pushnil(L);
			return 1;
		}
		GameObject*go = GetSceneManager()->mRootObject;
		const char*childName = lua_tostring(L, 1);
		GameObject*target = go->FindChild(childName);
		if (target != nullptr)
		{
			target->PushUserData(L);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedGameObject::API_Show(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->Show();
		return 0;
	}

	int ExportedGameObject::API_Hide(lua_State*L){
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		ptr->Hide();
		return 0;
	}

	int ExportedGameObject::API_CaptureTouchPos(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		if (lua_toboolean(L, 2)) {
			GetEventDispatcher()->CaptureTouchPos(ptr);
		}
		else {
			GetEventDispatcher()->ReleaseTouchPos(ptr);
		}
		return 0;
	}

	int ExportedGameObject::API_New(lua_State*L){
		int nArgCount = lua_gettop(L);
		if (nArgCount == 1){
			//string
			if (lua_isstring(L, 1)){
				const char*userPath = lua_tostring(L, 1);
				//init a inner prefab
				GetSceneManager()->mRootObject->NewGameObjectWithPrefabName(userPath);
			}else if (lua_isuserdata(L, 1)){
				//init via a given prefab
				PrefabResource*prefab = TO_USERDATA(L, PrefabResource, 1);
				GetSceneManager()->mRootObject->NewGameObjectWithPrefab(prefab);
			}
		}
		else
		{
			GetSceneManager()->mRootObject->NewGameObject();
		}
		return 1;
	}

	int ExportedGameObject::API_Destroy(lua_State*L){
		if (lua_gettop(L) != 1){
			errorC("ExportedGameObject.Destroy,arg 1 must be a game object");
			luaL_error(L, "GameObject.Destroy,arg 1 must be a game object");
			return 0;
		}
		if (IsType<GameObject>(L,1)){
			ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
			ptr->mObjectMask = -2;
			ptr->OnDestroy();
		}else{
			luaL_error(L, "GameObject.Destroy,arg 1 must be a game object,current arg at index 1 is %s", lua_typename(L, lua_type(L, 1)));
		}
		return 0;
	}
	int ExportedGameObject::API_InternalRoot(lua_State*L) {
		GetSceneManager()->mRootObject->PushUserData(L);
		return 1;
	}

	int ExportedGameObject::API_Dump(lua_State*L) {
		Info("game object dump -----------------------start");
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		Info("game object : %s", ptr->mName.mText);
		ptr->mLocalTransform.Dump();
		ptr->mLocalMatrix.Dump("Local Matrix");
		ptr->mWorldMatrix.Dump("World Matrix");
		ScriptObject*script = ptr->mScripts;
		while (script!=nullptr){
			Info("script component:%s",script->mName.mText);
			script = script->Next<ScriptObject>();
		}
		Info("game object dump -----------------------end");
		return 0;
	}
    
    int ExportedGameObject::API_DumpTree(lua_State *L){
        GetSceneManager()->mRootObject->DumpTree(0);
        return 0;
    }
	
	int ExportedGameObject::API_GetWorldDirection(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		const Matrix4x4 &world_matrix = ptr->GetWorldMatrix();
		lua_tonumber(L, world_matrix.mData[8]);
		lua_tonumber(L, world_matrix.mData[9]);
		lua_tonumber(L, world_matrix.mData[10]);
		return 3;
	}
	int ExportedGameObject::API_GetWorldUp(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		const Matrix4x4 &world_matrix = ptr->GetWorldMatrix();
		lua_tonumber(L, world_matrix.mData[4]);
		lua_tonumber(L, world_matrix.mData[5]);
		lua_tonumber(L, world_matrix.mData[6]);
		return 3;
	}
	int ExportedGameObject::API_GetWorldRight(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		const Matrix4x4 &world_matrix = ptr->GetWorldMatrix();
		lua_tonumber(L, world_matrix.mData[0]);
		lua_tonumber(L, world_matrix.mData[1]);
		lua_tonumber(L, world_matrix.mData[2]);
		return 3;
	}
	int ExportedGameObject::API_GetWorldPosition(lua_State*L) {
		ExportedGameObject*ptr = TO_USERDATA(L, ExportedGameObject, 1);
		const Matrix4x4 &world_matrix = ptr->GetWorldMatrix();
		lua_tonumber(L, world_matrix.mData[12]);
		lua_tonumber(L, world_matrix.mData[13]);
		lua_tonumber(L, world_matrix.mData[14]);
		return 3;
	}
	int ExportedGameObject::Export(lua_State*L)
	{
		luaL_Reg apim[] = {
			EXPORT_API(AddComponent)
			EXPORT_API(GetComponent)
			EXPORT_API(RemoveComponent)
			EXPORT_API(EnableKeyboardEvent)
			EXPORT_API(EnablePasteEvent)
			EXPORT_API(EnableIAPEvent)
			EXPORT_API(EnableAdRewardEvent)
			EXPORT_API(EnableRightMouseEvent)
			EXPORT_API(EnableMiddleMouseEvent)
			EXPORT_API(EnableReceiveRay)

			EXPORT_API(AttachScript)
			EXPORT_API(GetScript)
			EXPORT_API(DetachScript)

			EXPORT_API(AddChild)
			EXPORT_API(FindChild)
			EXPORT_API(InsertBefore)
			EXPORT_API(InsertAfter)

			EXPORT_API(SetPosition)
			EXPORT_API(SetRotation)
			EXPORT_API(GetName)
			EXPORT_API(SetName)
			EXPORT_API(SetLayer)
			EXPORT_API(GetWorldPosition)
			EXPORT_API(GetWorldDirection)
			EXPORT_API(GetWorldUp)
			EXPORT_API(GetWorldRight)
			EXPORT_API(GetWorldToModelMatrix)
			EXPORT_API(GetModelMatrix)

			EXPORT_API(GetPosition)
			EXPORT_API(GetRotation)
			EXPORT_API(GetScale)
			EXPORT_API(SetScale)
			EXPORT_API(SetShearing)
			EXPORT_API(GetShearing)
			EXPORT_API(Show)
			EXPORT_API(Hide)
			EXPORT_API(CaptureTouchPos)
			EXPORT_API(Dump)
			EXPORT_API(Call)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, GameObject, apim);
		luaL_Reg api_c[] = {
			EXPORT_API(New)
			EXPORT_API(Destroy)
#ifndef ALICE_FRIEND_COMPANY
			EXPORT_API(InternalRoot)
#endif
            EXPORT_API(Find)
            EXPORT_API(DumpTree)
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, GameObject, api_c);
		return 0;
	}
}
