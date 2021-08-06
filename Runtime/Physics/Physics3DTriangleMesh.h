#pragma once
#include "Physics3DComponent.h"
namespace Alice{
	class Physics3DTriangleMesh:public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DTriangleMesh)
	public:
		Physics3DTriangleMesh();
		virtual ~Physics3DTriangleMesh();
	};
}