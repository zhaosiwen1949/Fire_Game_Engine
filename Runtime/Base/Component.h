#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/Allocator/DefaultAllocator.h"

#define UPDATER(deltaTime) void Update(float deltaTime)
#if ALICE_EDITOR
#define RENDER(dc) void Render(RenderQueue*rq, DrawCallInfo &rs)
#define IMP_RENDER(t) void t::Render(RenderQueue*rq, DrawCallInfo &rs)
#else
#define IMP_RENDER(t) void t::Render(RenderQueue*rq)
#define RENDER(dc) void Render(RenderQueue*rq)
#endif
namespace Alice{
	class GameObject;
	class DrawCall;
	class RenderQueue;
#if ALICE_EDITOR
	class DrawCallInfo;
#endif
	enum ComponentType{
		CT_Transform = 1,
		CT_Object2D=2,
		CT_ImageSprite=3,
		CT_ImageSprite9=4,
		CT_AudioSource=5,
		CT_DynamicFont=6,
		CT_Label=7,
		CT_ComponentTypeCount
	};
	class Component:public Object,public DoubleLinkedList{
	public:
		DECLEAR_ALICE_CLASS(Component)
		Component();
		virtual ~Component();
		bool mbEnable;
		bool mbPositionChanged;
		void SetOwner(GameObject*go,bool is_script=false);
		GameObject*mOwner;
		void* PrepareUpdateMaterial(void*mat);
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		virtual void RenderShadowMap(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		virtual void Enable();
		virtual void Disable();
		bool IsEnabled();
		static int Export(lua_State*L);
		DEFINE_LUA_API(GetClassID);
		DEFINE_LUA_API(Enable);
		DEFINE_LUA_API(GameObject);
		//return one clone of the exactly one copy of this component
		virtual Component*Clone();
	public:
		void InitComponent(lua_State*L);
	};
}
