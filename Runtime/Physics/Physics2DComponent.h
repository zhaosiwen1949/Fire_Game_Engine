#pragma once
#include "PhysicsComponent.h"

namespace Alice
{
	class Physics2DComponent:public PhysicsComponent
	{
	public:
		DECLEAR_ALICE_CLASS(Physics2DComponent)
	public:
		Physics2DComponent();
		virtual ~Physics2DComponent();
		b2Body *mBody;
		b2Fixture*mFixture;
		static float mWorldScale;
		void OnCollideBegin(PhysicsComponent*other);
		void OnCollideEnd(PhysicsComponent*other);
		void SetCatagoryBits(AliceUInt16 bits);
		AliceUInt16 GetCatagoryBits();
		void SetMaskBits(AliceUInt16 bits);
		AliceUInt16 GetMaskBits();
		void SetCollideMask(AliceUInt16 catagory, AliceUInt16 mask);
		virtual void SyncDataFromLogicEffect();
		virtual void SyncDataToLogical();
	};
}