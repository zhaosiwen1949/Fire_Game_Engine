#pragma once
#include "Runtime/Base/Component.h"
#include "External/Box2D/Box2D/Box2D.h"
namespace Alice{
	enum PhysicsComponentType {
		kPhysicsComponentTypeUnkown,
		kPhysicsComponentType2D,
		kPhysicsComponentType2DBox,
		kPhysicsComponentType2DChain,
		kPhysicsComponentType2DCircle,
		kPhysicsComponentType2DEdge,
		kPhysicsComponentType3D,
		kPhysicsComponentType3DCapsule,
		kPhysicsComponentType3DCube,
		kPhysicsComponentType3DSphere,
		kPhysicsComponentType3DPlane,
		kPhysicsComponentType3DTriangleMesh,
		kPhysicsComponentType3DHeightField,
		kPhysicsComponentTypeCount
	};
	class PhysicsComponent : public Component{
	public:
		DECLEAR_ALICE_CLASS(PhysicsComponent)
	public:
		PhysicsComponentType mPhysicsComponentType;
		PhysicsComponent();
	public:
		virtual void SyncDataToLogical() {}
		virtual void SyncDataFromLogicEffect() {}
		virtual void OnCollideBegin(PhysicsComponent*other);
		virtual void SetCatagoryBits(AliceUInt16 bits);
		virtual void SetMaskBits(AliceUInt16 bits);
		virtual void SetCollideMask(AliceUInt16 catagory, AliceUInt16 mask);
		virtual void OnPhysicBodyChanged() {};
	};
}