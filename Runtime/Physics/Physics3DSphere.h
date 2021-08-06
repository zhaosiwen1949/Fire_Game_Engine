#pragma once
#include "Physics3DComponent.h"
#include "Runtime/Math/Vector3.h"
#include "Runtime/Geometry/Sphere.h"
namespace Alice{
	class Physics3DSphere:public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DSphere)
	public:
		Physics3DSphere();
		virtual ~Physics3DSphere();
		void SetOffset(float x, float y, float z);
		void SetRadius(float radius);
	};
}