#include "Runtime/Scene/GameObject.h"
namespace Alice{
	void GameObject::SyncPhysicsComponentRecusively(float deltaTime) {
		if (mbEnable) {//if game object is enabled
			if (mPhysicsComponent!=nullptr){
			}
			if (mChild != nullptr) {
				Child<GameObject>()->SyncPhysicsComponentRecusively(deltaTime);
			}
		}
		if (mRightSibling != nullptr) {
			RightSibling<GameObject>()->SyncPhysicsComponentRecusively(deltaTime);
		}
	}
	void GameObject::FixedSyncPhysicsToLogicalRecusively(float deltaTime) {
		if (mbEnable) {//if game object is enabled
			if (mPhysicsComponent != nullptr) {
			}
			if (mChild != nullptr) {
				Child<GameObject>()->FixedSyncPhysicsToLogicalRecusively(deltaTime);
			}
		}
		if (mRightSibling != nullptr) {
			RightSibling<GameObject>()->FixedSyncPhysicsToLogicalRecusively(deltaTime);
		}
	}
}