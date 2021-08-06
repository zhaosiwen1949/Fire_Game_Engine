#pragma once
#include "Runtime/Math/Vector3.h"
#include "PhysicsComponent.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
namespace Alice{
	class Physics3DComponent:public PhysicsComponent{
	public:
		Physics3DComponent();
		virtual ~Physics3DComponent();
		physx::PxRigidActor*mRigidActor;
		physx::PxShape* mShape;
		Vector3f mRigidOffset;
		void OnPhysicBodyChanged();
		virtual void SyncDataFromLogicEffect();
		virtual void SyncDataToLogical();
	public:
		void LockAngularX(bool v);
		void LockAngularY(bool v);
		void LockAngularZ(bool v);
		void LockAngular(bool x,bool y,bool z);
		void LockLinearX(bool v);
		void LockLinearY(bool v);
		void LockLinearZ(bool v);
		void LockLinear(bool x, bool y, bool z);
	public:
		template<typename T>
		T* RigidActor() {
			if (mRigidActor == nullptr) {
				return nullptr;
			}
			return (T*)mRigidActor;
		}
		static physx::PxMaterial *mDefaultMaterial;
	};
}