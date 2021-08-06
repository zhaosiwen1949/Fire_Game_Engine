#include "Physics3DCapsule.h"
#include "PhysicsManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Physics3DCapsule::Physics3DCapsule(){
	}

	Physics3DCapsule::~Physics3DCapsule(){
	}
	void Physics3DCapsule::SetInfo(float radius, float halfhegiht) {
		physx::PxCapsuleGeometry capsule;
		capsule.radius = radius;
		capsule.halfHeight = halfhegiht;
		mShape->setGeometry(capsule);
	}
}