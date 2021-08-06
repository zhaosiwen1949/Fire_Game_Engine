#pragma once
#include "Runtime/Base/Component.h"
#include "Box2D/Box2D.h"
#include "Runtime/Serializer/Physics.serializer.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
namespace Alice{
	class Physics2DComponent;
	class PhysicsManager:public b2ContactListener,public b2ContactFilter{
	public:
		//在这里更新所有节点的物理状态
		bool FixedUpdate(float deltaTime);
		b2World *m2DPhysicWorld;
		void Init(void*data = nullptr);
		void ShutDown();
		AliceUInt16 GetMaskByLayerID(AliceUInt32 layerID);
		void AddPhysicsMask(AliceUInt32 layerID, AliceUInt32 mask);
		void DeletePhysicsMask(AliceUInt32 layerID);
		void ClearMasks();
		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);
	public:
		static Physics2DComponent* CreatePhysics2DComponent();
	public:
		static physx::PxPhysics*mPhysxEngine;
		static physx::PxFoundation *mPhysxEngineFoundation;
		static physx::PxCooking *mPhysxEngineCooking;
		static physx::PxScene*mPhysxScence;
		static float mAccTimeEscapted;
		static float mFixedUpdateTime;
	};
}
Alice::PhysicsManager&GetPhysicsManager();