#pragma once
#include "Physics2DComponent.h"

namespace Alice
{
	class Physics2DBox:public Physics2DComponent
	{
	public:
		DECLEAR_ALICE_CLASS(Physics2DBox)
	public:
		Physics2DBox();
		virtual ~Physics2DBox();
		b2Vec2 mSize;
		void SetSize(float width, float height);
	};
}