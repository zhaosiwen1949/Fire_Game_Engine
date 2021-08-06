#include "Physics3DCube.h"
#include "PhysicsManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Physics3DCube::Physics3DCube(){
	}
	Physics3DCube::~Physics3DCube(){
	}
	void Physics3DCube::SetSize(float x, float y, float z) {
		physx::PxBoxGeometry box;
		box.halfExtents.x = x / 2.0f;
		box.halfExtents.y = y / 2.0f;
		box.halfExtents.z = z / 2.0f;
		mShape->setGeometry(box);
	}
}