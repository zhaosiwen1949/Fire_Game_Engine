#include "ExportedGameObject.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DBox.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DChain.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DCircle.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DComponent.h"
#include "Runtime/Exported/Physics/ExportedPhysics2DEdge.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DCapsule.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DComponent.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DCube.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DHeightField.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DConvexMesh.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DSphere.h"
#include "Runtime/Exported/Physics/ExportedPhysics3DTriangleMesh.h"
#include "Runtime/2D/Component2D.h"
#include "Runtime/2D/ImageSprite.h"
#include "Runtime/2D/ImageSprite9.h"
#include "Runtime/2D/Label.h"
namespace Alice {
	Physics2DBox*ExportedGameObject::AddPhysics2DBox(void*external) {
		ExportedPhysics2DBox*p2d = ExportedPhysics2DBox::New();
		p2d->SetOwner(this);
		Alice::Component2D*r2d = GetComponent<ImageSprite>();
		//if have a renderable component,then init size with the renderable component size
		if (r2d == nullptr)
		{
			r2d = GetComponent<ImageSprite9>();
			if (r2d == nullptr)
			{
				r2d = GetComponent<Label>();
			}
		}
		p2d->SetCollideMask((AliceUInt16)mLayer, GetPhysicsManager().GetMaskByLayerID(mLayer));
		if (r2d != nullptr)
		{
			p2d->SetSize(64, 64);
			//p2d->SetSize(r2d->mBox.mPoints[1].x - r2d->mBox.mPoints[0].x, r2d->mBox.mPoints[3].y - r2d->mBox.mPoints[0].y);
		}
		else
		{
			p2d->SetSize(100, 100);
		}
		return p2d;
	}
	Physics2DCircle*ExportedGameObject::AddPhysics2DCircle(void*external) {
		ExportedPhysics2DCircle*p2d = ExportedPhysics2DCircle::New();
		p2d->SetOwner(this);
		p2d->SetCollideMask((AliceUInt16)mLayer, GetPhysicsManager().GetMaskByLayerID(mLayer));
		return p2d;
	}
	Physics2DEdge*ExportedGameObject::AddPhysics2DEdge(void*external) {
		ExportedPhysics2DEdge*p2d = ExportedPhysics2DEdge::New();
		p2d->SetOwner(this);
		p2d->SetCollideMask((AliceUInt16)mLayer, GetPhysicsManager().GetMaskByLayerID(mLayer));
		return p2d;
	}
	Physics2DChain*ExportedGameObject::AddPhysics2DChain(void*external) {
		ExportedPhysics2DChain*p2d = ExportedPhysics2DChain::New();
		p2d->SetOwner(this);
		p2d->SetCollideMask((AliceUInt16)mLayer, GetPhysicsManager().GetMaskByLayerID(mLayer));
		return p2d;
	}
	Physics3DConvexMesh*ExportedGameObject::AddPhysics3DConvexMesh(void*external) {
		ExportedPhysics3DConvexMesh*ptr = ExportedPhysics3DConvexMesh::New();
		ptr->SetOwner(this);
		this->mPhysicsComponent = ptr;
		return ptr;
	}
	Physics3DCube*ExportedGameObject::AddPhysics3DCube(void*external) {
		ExportedPhysics3DCube*ptr = ExportedPhysics3DCube::New();
		ptr->SetOwner(this);
		ptr->mShape->userData = this;
		this->mPhysicsComponent = ptr;
		return ptr;
	}
	Physics3DSphere*ExportedGameObject::AddPhysics3DSphere(void*external) {
		ExportedPhysics3DSphere*ptr = ExportedPhysics3DSphere::New();
		ptr->SetOwner(this);
		ptr->mShape->userData = this;
		this->mPhysicsComponent = ptr;
		return ptr;
	}
	Physics3DCapsule*ExportedGameObject::AddPhysics3DCapsule(void*external) {
		ExportedPhysics3DCapsule*ptr = ExportedPhysics3DCapsule::New();
		ptr->SetOwner(this);
		ptr->mShape->userData = this;
		this->mPhysicsComponent = ptr;
		return ptr;
	}
	Physics3DTriangleMesh*ExportedGameObject::AddPhysics3DTriangleMesh(void*external) {
		ExportedPhysics3DTriangleMesh*ptr = ExportedPhysics3DTriangleMesh::New(external);
		ptr->SetOwner(this);
		ptr->mShape->userData = this;
		this->mPhysicsComponent = ptr;
		return ptr;
	}
	Physics3DHeightField*ExportedGameObject::AddPhysics3DHeightField(void*external) {
		ExportedPhysics3DHeightField*ptr = ExportedPhysics3DHeightField::New(external);
		ptr->SetOwner(this);
		ptr->mShape->userData = this;
		this->mPhysicsComponent = ptr;
		return ptr;
	}
}
