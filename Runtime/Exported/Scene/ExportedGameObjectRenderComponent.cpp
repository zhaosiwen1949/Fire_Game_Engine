#include "ExportedGameObject.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Exported/Component/ExportedAudioSource.h"
#include "Runtime/Exported/Component/ExportedCustomComponent.h"
#include "Runtime/Exported/Component/ExportedImageSprite.h"
#include "Runtime/Exported/Component/ExportedImageSprite9.h"
#include "Runtime/Exported/Component/ExportedLabel.h"
#include "Runtime/Exported/2D/ExportedSprite2D.h"
#include "Runtime/Exported/2D/ExportedSprite2D9.h"
#include "Runtime/Exported/2D/ExportedCanvas.h"
#include "Runtime/Exported/2D/ExportedClipArea2D.h"
#include "Runtime/Exported/3D/ExportedAvatar.h"
#include "Runtime/Exported/3D/ExportedMesh.h"
#include "Runtime/Exported/3D/ExportedMeshRenderer.h"
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
#include "Runtime/Exported/Render/ExportedRenderOrder.h"
#include "Runtime/Exported/Render/ExportedCamera.h"
#include "Runtime/Exported/Render/ExportedLighting.h"

namespace Alice {
	Component*ExportedGameObject::AddComponent(int componentID, void*external){
		Component*component = nullptr;
		switch (componentID){
		case ImageSprite::ClassID:
			component = AddImageSprite();
			break;
		case Camera::ClassID:
			component = AddCamera();
			break;
		case ImageSprite9::ClassID:
			component = AddImageSprite9();
			break;
		case Label::ClassID:
			component = AddLabel(external);
			break;
		case Physics2DBox::ClassID:
			component = AddPhysics2DBox();
			break;
		case Physics2DCircle::ClassID:
			component = AddPhysics2DCircle();
			break;
		case Physics2DEdge::ClassID:
			component = AddPhysics2DEdge();
			break;
		case Physics2DChain::ClassID:
			component = AddPhysics2DChain();
			break;
		case Physics3DConvexMesh::ClassID:
			component = AddPhysics3DConvexMesh();
			break;
		case Physics3DCube::ClassID:
			component = AddPhysics3DCube();
			break;
		case Physics3DSphere::ClassID:
			component = AddPhysics3DSphere();
			break;
		case Physics3DCapsule::ClassID:
			component = AddPhysics3DCapsule();
			break;
		case Physics3DTriangleMesh::ClassID:
			component = AddPhysics3DTriangleMesh();
			break;
		case Physics3DHeightField::ClassID:
			component = AddPhysics3DHeightField();
			break;
		case AudioSource::ClassID:
			component = AddAudioSource();
			break;
		case CustomComponent::ClassID:
			component = CreateExportedAliceType<ExportedCustomComponent>();
			component->SetOwner(this);
			break;
		case Mesh::ClassID:
			component = AddMesh();
			break;
		case Avatar::ClassID:
			component = AddAvatar();
			break;
		case MeshRenderer::ClassID:
			component = AddMeshRenderer();
			break;
		case Terrain::ClassID:
			component = AddTerrain();
			break;
		case Light::ClassID:
			component = AddLight();
			break;
		case RenderOrder::ClassID:
			component = AddRenderOrder();
			break;
		case Sprite2D::ClassID:
			component = AddSprite2D();
			break;
		case Sprite2D9::ClassID:
			component = AddSprite2D9();
			break;
		case Canvas::ClassID:
			component = AddCanvas();
			break;
		case ClipArea2D::ClassID:
			component = AddClipArea2D();
			break;
		}
		return component;
	}

	Sprite2D*ExportedGameObject::AddSprite2D(void*external) {
		Sprite2D*sprite2d = CreateExportedAliceType<ExportedSprite2D>();
		sprite2d->SetOwner(this);
		return sprite2d;
	}
	Sprite2D9*ExportedGameObject::AddSprite2D9(void*external) {
		Sprite2D9*sprite2d = CreateExportedAliceType<ExportedSprite2D9>();
		sprite2d->SetOwner(this);
		return sprite2d;
	}
	Canvas*ExportedGameObject::AddCanvas(void*external) {
		Canvas*canvas = CreateExportedAliceType<ExportedCanvas>();
		canvas->SetOwner(this);
		return canvas;
	}
	ClipArea2D*ExportedGameObject::AddClipArea2D(void*external) {
		ClipArea2D*ca2d = CreateExportedAliceType<ExportedClipArea2D>();
		ca2d->SetOwner(this);
		return ca2d;
	}
	ImageSprite*ExportedGameObject::AddImageSprite(void*external){
		ImageSprite*is = CreateExportedAliceType<ExportedImageSprite>();
		is->SetOwner(this);
		return is;
	}
	RenderOrder*ExportedGameObject::AddRenderOrder(void*external) {
		RenderOrder*ro = CreateExportedAliceType<ExportedRenderOrder>();
		ro->SetOwner(this);
		return ro;
	}
	ImageSprite9*ExportedGameObject::AddImageSprite9(void*external){
		ImageSprite9*is = CreateExportedAliceType<ExportedImageSprite9>();
		is->SetOwner(this);
		return is;
	}
	Label*ExportedGameObject::AddLabel(void*external){
		Label*label = CreateExportedAliceType<ExportedLabel>();
		int maxCharCount = 128;
		if (external!=nullptr){
			maxCharCount = lua_tointeger((lua_State*)external, -2);
		}
		label->Initialize(maxCharCount);
		label->SetOwner(this);
		label->InitWithDefaultText();
		return label;
	}
	Camera*ExportedGameObject::AddCamera(void*external){
		ExportedCamera*component = CreateExportedAliceType<ExportedCamera>();
		component->SetOwner(this);
		GetSceneManager()->AddCamera(component);
		int width, height;
		GetSceneManager()->GetCanvasSize(width, height);
		component->OnViewPortSizeChange(width, height);
		return component;
	}

	AudioSource*ExportedGameObject::AddAudioSource(void*external){
		AudioSource*as = CreateExportedAliceType<ExportedAudioSource>();
		as->SetOwner(this);
		return as;
	}

	Mesh*ExportedGameObject::AddMesh(void*external){
		Mesh*mesh = CreateExportedAliceType<ExportedMesh>();
		mesh->SetOwner(this);
		return mesh;
	}

	Avatar*ExportedGameObject::AddAvatar(void*external)
	{
		Avatar*avatar = CreateExportedAliceType<ExportedAvatar>();
		avatar->SetOwner(this);
		return avatar;
	}

	MeshRenderer*ExportedGameObject::AddMeshRenderer(void*external)
	{
		MeshRenderer*mesh_renderer = CreateExportedAliceType<ExportedMeshRenderer>();
		mesh_renderer->SetOwner(this);
		return mesh_renderer;
	}

	Terrain*ExportedGameObject::AddTerrain(void*external) {
		Terrain*ptr = CreateExportedAliceType<ExportedTerrain>();
		ptr->SetOwner(this);
		return ptr;
	}

	Light*ExportedGameObject::AddLight(void*external)
	{
		Light*light = CreateExportedAliceType<ExportedLight>();
		light->SetOwner(this);
		Light::mLights.insert(light);
		return light;
	}
}