#pragma once
#include "Physics3DComponent.h"
namespace Alice{
	class Physics3DHeightField:public Physics3DComponent {
	public:
		DECLEAR_ALICE_CLASS(Physics3DHeightField)
	public:
		Physics3DHeightField();
		virtual ~Physics3DHeightField();
	};
}