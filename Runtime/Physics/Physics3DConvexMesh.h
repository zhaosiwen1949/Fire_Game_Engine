#pragma once
#include "Physics3DComponent.h"
namespace Alice{
	class Physics3DConvexMesh :public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DConvexMesh)
	public:
		Physics3DConvexMesh();
		virtual ~Physics3DConvexMesh();
	};
}