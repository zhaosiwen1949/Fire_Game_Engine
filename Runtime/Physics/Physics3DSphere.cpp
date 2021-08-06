#include "Physics3DSphere.h"
#include "PhysicsManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Physics3DSphere::Physics3DSphere(){
	}
	Physics3DSphere::~Physics3DSphere(){
	}
	void Physics3DSphere::SetOffset(float x, float y, float z) {
		mRigidOffset.Set(x, y, z);
	}
	void Physics3DSphere::SetRadius(float radius) {
		physx::PxSphereGeometry geometry;
		mShape->getSphereGeometry(geometry);
		geometry.radius = radius;
		mShape->setGeometry(geometry);
	}
}