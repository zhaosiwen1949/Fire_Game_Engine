#include "Runtime/Base/Component.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
namespace Alice{
	Component::Component() :mOwner(nullptr),
		mbPositionChanged(true),
		mbEnable(true){
	}
	Component::~Component(){
	}
	void* Component::PrepareUpdateMaterial(void*mat) {
		Material*ptr = (Material*)mat;
		if (ptr->mbSharedMaterial) {
			//clone a material
			Material*ret = CreateExportedAliceType<ExportedMaterial>();
			*ret = *ptr;
			ptr->release();
			ptr = ret;
			ptr->mbSharedMaterial = false;
			ptr->retain();
		}
		return ptr;
	}
	void Component::Update(float deltaTime){
		//Debug("inside component update method");
	}
	void Component::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	){
	}
	void Component::RenderShadowMap(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
	}
	Component*Component::Clone(){
		return NULL;
	}

	void Component::Disable(){
		if (!mbEnable){
			return;
		}
		mbEnable = false;
	}

	void Component::Enable(){
		if (mbEnable){
			return;
		}
		mbEnable = true;
	}

	bool Component::IsEnabled(){
		return mbEnable&&mOwner->IsEnabled();
	}

	void Component::SetOwner(GameObject*go, bool is_script){
		mOwner = go;
		if (is_script) {
			if (mOwner->mScripts == nullptr) {
				mOwner->mScripts = (ScriptObject*)this;
			}
			else {
				mOwner->mScripts->Append(this);
			}
		}
		else {
			go->AddComponent(this);
		}
	}

	int Component::Export(lua_State*L){
		luaL_Reg api_m[] = {
			EXPORT_API(GameObject)
			EXPORT_API(Enable)
			EXPORT_API(GetClassID)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, Component, api_m);
		return 0;
	}

	int Component::API_GetClassID(lua_State*L){
		Component*ptr = TO_USERDATA(L,Component,1);
		lua_pushinteger(L, ptr->GetClassID());
		return 1;
	}
	
	int Component::API_Enable(lua_State*L) {
		Component*ptr = TO_USERDATA(L, Component, 1);
		if (lua_toboolean(L, 2)){
			ptr->Enable();
		}else{
			ptr->Disable();
		}
		return 0;
	}

	int Component::API_GameObject(lua_State*L) {
		Component*ptr = TO_USERDATA(L, Component, 1);
		if (ptr->mOwner!=nullptr){
			ptr->mOwner->PushUserData(L);
		}
		return 1;
	}

	void Component::InitComponent(lua_State*L){
		mLuaData = luaL_ref(L,LUA_REGISTRYINDEX);
		lua_getref(L, mLuaData);
	}
}
