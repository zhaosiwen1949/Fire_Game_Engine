#include "Physics3DComponent.h"
#include "PhysicsManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	physx::PxMaterial*Physics3DComponent::mDefaultMaterial = nullptr;
	Physics3DComponent::Physics3DComponent() {
		mRigidActor = nullptr;
		mShape = nullptr;
		mPhysicsComponentType = kPhysicsComponentType3D;
	}
	Physics3DComponent::~Physics3DComponent() {
		if (mRigidActor!=nullptr){
			GetPhysicsManager().mPhysxScence->removeActor(*mRigidActor);
			if (mShape!=nullptr){
				mRigidActor->detachShape(*mShape);
				mShape->release();
			}
			mRigidActor->release();
		}
	}
	void Physics3DComponent::LockAngular(bool x, bool y, bool z) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);
	}
	void Physics3DComponent::LockAngularX(bool x) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
	}
	void Physics3DComponent::LockAngularY(bool y) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
	}
	void Physics3DComponent::LockAngularZ(bool z) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);
	}
	void Physics3DComponent::LockLinear(bool x, bool y, bool z) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, x);
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, y);
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, z);
	}
	void Physics3DComponent::LockLinearX(bool x) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, x);
	}
	void Physics3DComponent::LockLinearY(bool y) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, y);
	}
	void Physics3DComponent::LockLinearZ(bool z) {
		physx::PxRigidDynamic*rigid = RigidActor<physx::PxRigidDynamic>();
		rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, z);
	}
	void Physics3DComponent::OnPhysicBodyChanged() {
		if (mRigidActor != nullptr) {
			physx::PxTransform transform = mRigidActor->getGlobalPose();
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			const float &gposx = world_matrix.mData[12] + mRigidOffset.x;
			const float &gposy = world_matrix.mData[13] + mRigidOffset.y;
			const float &gposz = world_matrix.mData[14] + mRigidOffset.z;
			if (!EqualApproximately(Alice::Alice_ABS(gposx - transform.p.x), 0.0f) ||
				!EqualApproximately(Alice::Alice_ABS(gposy - transform.p.y), 0.0f) ||
				!EqualApproximately(Alice::Alice_ABS(gposz - transform.p.z), 0.0f)) {
				//Debug("Physics3DComponent::OnPhysicBodyChanged %f,%f,%f",transform.p.x,transform.p.y,transform.p.z);
				mOwner->SetPosition(transform.p.x - mRigidOffset.x, transform.p.y - mRigidOffset.y, transform.p.z - mRigidOffset.z);
				mOwner->mLocalTransform.mbAffectedByPhysics = true;
			}
		}
	}
	void Physics3DComponent::SyncDataFromLogicEffect() {
		if (mbPositionChanged) {
			if (mRigidActor != nullptr) {
				physx::PxTransform transform = mRigidActor->getGlobalPose();
				const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
				Vector3f center(
					world_matrix.mData[12] + mRigidOffset.x,
					world_matrix.mData[13] + mRigidOffset.y,
					world_matrix.mData[14] + mRigidOffset.z);
				if (!EqualApproximately(Alice::Alice_ABS(center.x - transform.p.x), 0.0f) ||
					!EqualApproximately(Alice::Alice_ABS(center.y - transform.p.y), 0.0f) ||
					!EqualApproximately(Alice::Alice_ABS(center.z - transform.p.z), 0.0f)) {
					transform.p.x = center.x;
					transform.p.y = center.y;
					transform.p.z = center.z;
                    if(transform.q.isFinite()==false){
                        transform.q.x=0.0f;
                        transform.q.y=0.0f;
                        transform.q.z=0.0f;
                        transform.q.w=1.0f;
                    }
                    if(transform.p.isFinite()==false){
                        return;
                    }
					mRigidActor->setGlobalPose(transform);
                    transform = mRigidActor->getGlobalPose();
				}
			}
		}
	}
	void Physics3DComponent::SyncDataToLogical() {
		if (mRigidActor != nullptr) {
			physx::PxTransform transform = mRigidActor->getGlobalPose();
            if(transform.p.isFinite()==false){
                return;
            }
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			const float &gposx = world_matrix.mData[12] + mRigidOffset.x;
			const float &gposy = world_matrix.mData[13] + mRigidOffset.y;
			const float &gposz = world_matrix.mData[14] + mRigidOffset.z;
			if (!EqualApproximately(Alice::Alice_ABS(gposx - transform.p.x), 0.0f) ||
				!EqualApproximately(Alice::Alice_ABS(gposy - transform.p.y), 0.0f) ||
				!EqualApproximately(Alice::Alice_ABS(gposz - transform.p.z), 0.0f)) {
				mOwner->SetPosition(transform.p.x - mRigidOffset.x, transform.p.y - mRigidOffset.y, transform.p.z - mRigidOffset.z);
				mOwner->mLocalTransform.mbAffectedByPhysics = true;
			}
		}
	}
}
