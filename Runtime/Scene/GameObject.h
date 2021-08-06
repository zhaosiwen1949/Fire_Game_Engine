#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Base/Component.h"
#include "Runtime/Base/Transform.h"
#include "Runtime/Base/ScriptObject.h"
#include "Runtime/LuaEngine/LuaCallback.h"
#include "Runtime/Geometry/AABB.h"
#include "Runtime/Scene/RendererManager.h"
#include "Runtime/Audio/AudioSource.h"
#include "Runtime/Scene/EventDispatcher.h"
#include "Runtime/Serializer/GameObject.serializer.h"
#include "Runtime/Utils/TTree.h"
#include "TouchEvent.h"
#include "Runtime/Geometry/Ray.h"
#include "Runtime/String/FixedString.h"
namespace Alice{
	class AudioSource;
	class Camera;
	class ImageSprite;
	class ImageSprite9;
	class Label;
	class Particle;
	class Mesh;
	class Avatar;
	class Model;
	class Light;
	class PrefabResource;
	class RenderOrder;
	class PhysicsComponent;

	enum TransformFeature{
		kNoScaleTransform = 0,
		kUniformScaleTransform = 1 << 0,
		kNonUniformScaleTransform = 1 << 1,
		kOddNegativeScaleTransform = 1 << 2
	};

	enum GameObjectLayer{
		GameObjectLayer_Default=0x01,
		GameObjectLayer_2D =0x02,
	};
	//有这个信息已经实时的视锥体信息，就可以做最基本的裁剪功能了。目前在做2D的引擎方面，所以暂且放置这个功能，我们能做到在平面裁剪即可
	struct TransformInfo{
		Matrix4x4     worldMatrix;     // 64
		AABB           worldAABB;       // 24
		AABB           localAABB;       // 24	used by LightManager and Shadows
		float          invScale;        // 4
		TransformFeature  transformType;   // 4
	};

	//OBB包围盒
	struct OBB{
		Vector3f mPoints[4];
	};
	//这个物体只是一个代理物体，它身上绑定了Component之后才有具体的意义
	//一个物体要挂载到场景树上，必须具备Transform属性，Transform或者Transform2D都可以
	class GameObject :public TTree,public Object{
	private:
		GameObject(const GameObject&r) {}
	public:
		DECLEAR_ALICE_CLASS(GameObject)
	public:
		//set world position
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void SetShearing(float x, float y, float z);
		void UpdateBoundingBox();
        void DumpTree(int nLevel);
		virtual void OnCollideBegin(GameObject*other);
		virtual void OnCollideEnd(GameObject*other);
		OPT(void) OnViewportChanged(float width,float height);
		virtual void OnStop();
		void Enable() { mbEnable = true; }
		void Disable() { mbEnable = false; }
		bool IsEnabled();
		GameObject();
		virtual ~GameObject();

		OPT(bool)NewGameObject();
		OPT(bool)NewGameObjectWithPrefabName(const char*prefabName);
		OPT(bool)NewGameObjectWithPrefab(PrefabResource*prefab);
		OPT(bool)DestroyGameObject(GameObject*go);

		template<typename T>
		T* GetComponent() {
			return (T*)GetComponent(T::ClassID);
		}
		Component*GetComponent(int componentID);

		OPT(bool)RemoveComponent(Component*component);
		void OnRemoveComponent(Component*component);
		void AddComponent(Component*component);

		OPT(bool)AttachScript(const char*scriptName);
		ScriptObject*GetScript(const char*scriptName);
		OPT(bool)DetachScript(ScriptObject*so);

		OPT(bool)AddChild(GameObject*go);
		OPT(bool)InsertAfter(GameObject*go);
		OPT(bool)InsertBefore(GameObject*go);

		OPT(bool)Show();
		OPT(bool)Hide();
		void OnDestroy();

		OPT(bool)SetLocalPosition(float x, float y, float z);
		OPT(bool)SetName(const char*name);
		OPT(bool)SetLocalRotation(float x, float y, float z);
		OPT(bool)SetLocalScale(float x, float y, float z);

		GameObject*FindChild(const char*childName);
		//always update transform one time every frame
		void UpdateTransformRecursively();
		//if game object is active ,update scripts one time every frame
		void UpdateScriptsRecursively(float deltaTime);
		//if game object is active ,update component one time every frame
		void UpdateComponentRecusively(float deltaTime);
		//if game object is active ,sync physics every frame
		void SyncPhysicsComponentRecusively(float deltaTime);
		//if game object is active ,sync physics every frame
		void FixedSyncPhysicsToLogicalRecusively(float deltaTime);
		//if game object is active ,render component maybe multi times per frame because the game object may be render by different camera
		void RenderComponentRecursively(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		, bool render_sibling = true);
		void RenderUIComponentRecursively(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
			, bool render_sibling = true);
		void RenderShadowMapRecursively(Camera*camera, RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		//render myself only
		void RenderMyselfOnly(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		RenderOrder*mRenderOrder;//if exist,the child will not be rendererd
		bool mbEnable;
		bool mbReceiveRayCast;
		bool mbWorldMatrixDirty;
		bool mbLocalMatrixDirty;
		void*mExtensionData;
		AliceUInt32 mLayer;
		Component*mComponents;
		ScriptObject*mScripts;
		//world matrix
		Transform mTPoseTransform;
		Matrix4x4 mWorldMatrix;
		//local matrix
		Transform mLocalTransform;
		Matrix4x4 mLocalMatrix;
		bool mbIsBone;
		PhysicsComponent*mPhysicsComponent;
	public://intersect
		const Matrix4x4&GetWorldMatrix();
		const Matrix4x4&GetLocalMatrix();
		void MarkWorldMatrixDirty();
		Alice::GameObject*Insetersect(TouchEvent *te, bool bEditMode = false);
		//RayCast
		Alice::GameObject*RayCastTestRecursively(Ray*ray);
		Alice::GameObject*RayCastTest(Ray*ray);
		OPT(GameObject*)Intersect2DComponent(float x, float y, bool bEditMode);
		OPT(void)InitSelfWithSerializedData(Serializer::GameObject*data, Material*material = nullptr);
		OPT(void)InitImageSprite9(const Serializer::ImageSprite9&image_sprite_data, Material*material = nullptr);
	public://event dispatch
		void OnTouchBegin(const TouchEvent&te);
		void OnTouchEnd(const TouchEvent&te);
		void OnTouchMove(const TouchEvent&te);
		void OnTouchCanceled(const TouchEvent&te);
		void OnTouchEnter(const TouchEvent&te);
		void OnTouchLeave(const TouchEvent&te);
		void OnMouseWheel(const MouseWheelEvent*event);
		void OnIMECompositionString(const char*utf8Str,bool isResult);
		void OnIMEChar(const char*utf8Str);
        void OnEditEnd();
		void OnChar(AliceUInt32 charCode);
		void OnKeyDown(int code);
		void OnKeyUp(int code);
		void OnPaste(const char*str, int len);
		void OnIAP(void*product);
		void OnAdReward(void*product);
		void PostRendering(Camera*camera);
		void Call(const char *method, Param *param);

		void OnAnimationEnd(const char*animationName);
	};
}
