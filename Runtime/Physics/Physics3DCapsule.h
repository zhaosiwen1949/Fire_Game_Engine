#pragma once
#include "Physics3DComponent.h"
namespace Alice{
	class Physics3DCapsule :public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DCapsule)
	public:
		Physics3DCapsule();
		virtual ~Physics3DCapsule();
		void SetInfo(float radius,float halfhegiht);
	};
}