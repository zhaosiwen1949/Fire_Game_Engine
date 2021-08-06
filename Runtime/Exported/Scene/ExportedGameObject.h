#pragma once
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Exported/3D/ExportedTerrain.h"
namespace Alice
{
	class Physics2DBox;
	class Physics2DCircle;
	class Physics2DEdge;
	class Physics2DChain;
	class Physics3DConvexMesh;
	class Physics3DCube;
	class Physics3DSphere;
	class Physics3DCapsule;
	class Physics3DTriangleMesh;
	class Physics3DHeightField;
	class Sprite2D;
	class Sprite2D9;
	class Canvas;
	class ClipArea2D;
	class MeshRenderer;
	class ExportedGameObject :public GameObject
	{
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(New);
		DEFINE_LUA_API(Destroy);
		DEFINE_LUA_API(InternalRoot);
		DEFINE_LUA_API(AddComponent);
		DEFINE_LUA_API(GetComponent);
		DEFINE_LUA_API(RemoveComponent);
		DEFINE_LUA_API(EnableKeyboardEvent);
		DEFINE_LUA_API(EnablePasteEvent);
		DEFINE_LUA_API(EnableIAPEvent);
		DEFINE_LUA_API(EnableAdRewardEvent);
		DEFINE_LUA_API(EnableRightMouseEvent);
		DEFINE_LUA_API(EnableMiddleMouseEvent);
		DEFINE_LUA_API(EnableReceiveRay);
		DEFINE_LUA_API(AttachScript);
		DEFINE_LUA_API(GetScript);
		DEFINE_LUA_API(DetachScript);
		DEFINE_LUA_API(Call);
		DEFINE_LUA_API(AddChild);
		DEFINE_LUA_API(InsertAfter);
		DEFINE_LUA_API(InsertBefore);
		DEFINE_LUA_API(SetPosition);
		DEFINE_LUA_API(SetRotation);
		DEFINE_LUA_API(SetScale);
		DEFINE_LUA_API(SetShearing);
		DEFINE_LUA_API(SetName);
		DEFINE_LUA_API(SetLayer);
		DEFINE_LUA_API(GetPosition);
		DEFINE_LUA_API(GetRotation);
		DEFINE_LUA_API(GetScale);
		DEFINE_LUA_API(GetWorldPosition);
		DEFINE_LUA_API(GetWorldDirection);
		DEFINE_LUA_API(GetWorldUp);
		DEFINE_LUA_API(GetWorldRight);
		DEFINE_LUA_API(GetWorldToModelMatrix);
		DEFINE_LUA_API(GetModelMatrix);
		DEFINE_LUA_API(GetShearing);
		DEFINE_LUA_API(GetName);
		DEFINE_LUA_API(FindChild);
		DEFINE_LUA_API(Find);
		DEFINE_LUA_API(Show);
		DEFINE_LUA_API(Hide);
		DEFINE_LUA_API(CaptureTouchPos);
        DEFINE_LUA_API(Dump);
        DEFINE_LUA_API(DumpTree);
	public:
		int AddComponentWithArg(lua_State*L);
		int AddComponentNoArg(lua_State*L);
		Component * AddComponent(int classID,void*external=nullptr);
		template<typename T>
		T* AddComponent() {
			return (T*)AddComponent(T::ClassID);
		}
		ImageSprite*AddImageSprite(void*external = nullptr);
		ImageSprite9*AddImageSprite9(void*external = nullptr);
		Label*AddLabel(void*external = nullptr);
		Camera*AddCamera(void*external = nullptr);
		AudioSource*AddAudioSource(void*external = nullptr);
		RenderOrder*AddRenderOrder(void*external = nullptr);

		Sprite2D*AddSprite2D(void*external = nullptr);
		Sprite2D9*AddSprite2D9(void*external = nullptr);
		Canvas*AddCanvas(void*external = nullptr);
		ClipArea2D*AddClipArea2D(void*external = nullptr);

		Mesh*AddMesh(void*external = nullptr);
		Avatar*AddAvatar(void*external = nullptr);
		MeshRenderer*AddMeshRenderer(void*external = nullptr);
		Terrain*AddTerrain(void*external = nullptr);

		Physics2DBox*AddPhysics2DBox(void*external = nullptr);
		Physics2DCircle*AddPhysics2DCircle(void*external = nullptr);
		Physics2DEdge*AddPhysics2DEdge(void*external = nullptr);
		Physics2DChain*AddPhysics2DChain(void*external = nullptr);

		Physics3DConvexMesh*AddPhysics3DConvexMesh(void*external = nullptr);
		Physics3DCube*AddPhysics3DCube(void*external = nullptr);
		Physics3DSphere*AddPhysics3DSphere(void*external = nullptr);
		Physics3DCapsule*AddPhysics3DCapsule(void*external = nullptr);
		Physics3DTriangleMesh*AddPhysics3DTriangleMesh(void*external = nullptr);
		Physics3DHeightField*AddPhysics3DHeightField(void*external = nullptr);

		Light*AddLight(void*external = nullptr);
	public:
		virtual void RestoreSelfWithSerializedData(Serializer::GameObject*data);
		virtual void InitSelfWithSerializedData(Serializer::GameObject*data,Material*material=nullptr);
		OPT(void)InitImageSprite(const Serializer::ImageSprite&image_sprite, Material*material = nullptr);
		OPT(void)InitImageSprite9(const Serializer::ImageSprite9&image_sprite_data, Material*material = nullptr);
		OPT(bool)AwakeScripts(Serializer::GameObject*goData);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedGameObject,GameObject)
	};
}
