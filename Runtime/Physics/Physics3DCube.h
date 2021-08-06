#pragma once
#include "Physics3DComponent.h"
namespace Alice{
	class Physics3DCube :public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DCube)
	public:
		Physics3DCube();
		virtual ~Physics3DCube();
		void SetSize(float x, float y, float z);
	};
}