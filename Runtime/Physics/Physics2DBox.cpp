#include "Physics2DBox.h"
#include "PhysicsManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Physics2DBox::Physics2DBox(){
	}
	Physics2DBox::~Physics2DBox(){
	}
	void Physics2DBox::SetSize(float width, float height){
		b2PolygonShape*shape = (b2PolygonShape*)mBody->GetFixtureList()->GetShape();
		shape->SetAsBox(mWorldScale* width / 2.0f, mWorldScale* height / 2.0f);
		mSize.Set(mWorldScale*width, mWorldScale*height);
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		mBody->SetTransform(b2Vec2(mWorldScale*world_matrix.mData[12], mWorldScale*world_matrix.mData[13]), mBody->GetAngle());
	}
}