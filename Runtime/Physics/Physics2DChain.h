#pragma once
#include "Physics2DComponent.h"

namespace Alice
{
	class Physics2DChain:public Physics2DComponent
	{
	public:
		DECLEAR_ALICE_CLASS(Physics2DChain)
	public:
		Physics2DChain();
		virtual ~Physics2DChain();
		b2ChainShape mShape;
		b2Vec2 *mVertexes;
		int mVertexCount;
	};
}