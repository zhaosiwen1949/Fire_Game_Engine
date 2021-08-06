#pragma once
#include "Physics2DComponent.h"
namespace Alice{
	class Physics2DCircle:public Physics2DComponent{
	public:
		DECLEAR_ALICE_CLASS(Physics2DCircle)
	public:
		Physics2DCircle();
		virtual ~Physics2DCircle();
	};
}