#pragma once
#include "Physics2DComponent.h"

namespace Alice
{
	class Physics2DEdge:public Physics2DComponent
	{
	public:
		DECLEAR_ALICE_CLASS(Physics2DEdge)
	public:
		Physics2DEdge();
		virtual ~Physics2DEdge();
		b2EdgeShape mShape;
	};
}