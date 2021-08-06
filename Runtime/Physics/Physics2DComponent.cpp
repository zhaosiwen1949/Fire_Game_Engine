#include "Physics2DComponent.h"
#include "PhysicsManager.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DComponent.h"
#include "Runtime/Scene/GameObject.h"

#if ALICE_EDITOR || ALICE_WIN_PLAYER
#pragma comment(lib,"Box2D.lib")
#endif
namespace Alice{
	float Physics2DComponent::mWorldScale = 0.02f;
	Physics2DComponent::Physics2DComponent(){
		mBody = nullptr;
		mFixture = nullptr;
		mPhysicsComponentType = kPhysicsComponentType2D;
	}
	Physics2DComponent::~Physics2DComponent(){
		GetPhysicsManager().m2DPhysicWorld->DestroyBody(mBody);
	}
	void Physics2DComponent::OnCollideBegin(PhysicsComponent*other){
		mOwner->OnCollideBegin(other->mOwner);
	}
	void Physics2DComponent::OnCollideEnd(PhysicsComponent*other){
		mOwner->OnCollideEnd(other->mOwner);
	}
	void Physics2DComponent::SyncDataFromLogicEffect() {
		if (mbPositionChanged) {
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			mBody->SetTransform(b2Vec2(mWorldScale*world_matrix.mData[12], mWorldScale*world_matrix.mData[13]), mOwner->mLocalTransform.mRotation.z*ALICE_PI_DIV_180);
		}
	}
	void Physics2DComponent::SyncDataToLogical() {
		if (mBody->IsAwake() > 0 && mBody->GetMass() > 0.0f) {
			const b2Vec2 & pos = mBody->GetPosition();
			mOwner->mLocalTransform.SetLocalPosition(pos.x / mWorldScale, pos.y / mWorldScale, mOwner->mLocalTransform.mPosition.z, true);
			mOwner->mLocalTransform.SetLocalRotation(0.0f, 0.0f, mBody->GetAngle()*ALICE_180_DIV_PI, true);
		}
	}
	void Physics2DComponent::SetCatagoryBits(AliceUInt16 bits){
		if (mFixture == nullptr) {
			return;
		}
		const b2Filter&of = mFixture->GetFilterData();
		b2Filter nf;
		nf.categoryBits = bits;
		nf.groupIndex = of.groupIndex;
		nf.maskBits = of.maskBits;
		mFixture->SetFilterData(nf);
	}
	AliceUInt16 Physics2DComponent::GetCatagoryBits(){
		if (mFixture == nullptr) {
			return 0xFFFF;
		}
		const b2Filter&of = mFixture->GetFilterData();
		return of.categoryBits;
	}
	void Physics2DComponent::SetMaskBits(AliceUInt16 bits){
		if (mFixture == nullptr) {
			return;
		}
		const b2Filter&of = mFixture->GetFilterData();
		b2Filter nf;
		nf.categoryBits = of.categoryBits;
		nf.groupIndex = of.groupIndex;
		nf.maskBits = bits;
		mFixture->SetFilterData(nf);
	}
	AliceUInt16 Physics2DComponent::GetMaskBits(){
		if (mFixture == nullptr) {
			return 0xFFFF;
		}
		const b2Filter&of = mFixture->GetFilterData();
		return of.maskBits;
	}
	void Physics2DComponent::SetCollideMask(AliceUInt16 catagory, AliceUInt16 mask){
		if (mFixture==nullptr){
			return;
		}
		const b2Filter&of = mFixture->GetFilterData();
		b2Filter nf;
		nf.categoryBits = catagory;
		nf.groupIndex = of.groupIndex;
		nf.maskBits = mask;
		mFixture->SetFilterData(nf);
	}
}