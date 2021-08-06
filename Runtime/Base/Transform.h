#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Math/Vector3.h"
#include "Runtime/Math/Quaternion.h"
#include "Runtime/Math/AliceMatrix4x4.h"
namespace Alice{
	enum TransformType{
		kTransformTypeNormal,
		kTransformTypeOnlyTranslation,
		kTransformTypeNoRotationOrReflection,
		kTransformTypeNoScale,
		kTransformTypeNoScaleOrReflection,
		kTransformTypeCount
	};
	class Transform : public Component{
	public:
		TransformType mType;
		Vector3f mScale;
		Vector3f mPosition;
		Vector3f mRotation;
		Vector3f mShearing;
		Quaternionf mLocalQuaternion;
		Transform();
		bool mbDirty;
		bool mbAffectedByPhysics;
	public:
		void SetLocalPosition(float x,float y,float z, bool affectedbyphysics=false);
		void SetLocalRotation(float x,float y,float z, bool affectedbyphysics = false);
		void SetLocalScale(float x, float y, float z);
		void SetLocalShearing(float x, float y, float z);
		void ToMatrix(Matrix4x4&matrix);
		void operator=(const Transform*r);
		void operator=(const Transform&r);
		void Dump(int level=0);
	private:
		DEFINE_LUA_API(SetPosition);
		DEFINE_LUA_API(GetPosition);
		DEFINE_LUA_API(SetRotation);
		DEFINE_LUA_API(GetRotation);
		DEFINE_LUA_API(SetScale);
		DEFINE_LUA_API(GetScale);
		DEFINE_LUA_API(SetShearing);
		DEFINE_LUA_API(GetShearing);
	public:
		static int Export(lua_State*L);
		DECLEAR_ALICE_CLASS(Transform)
	};
}