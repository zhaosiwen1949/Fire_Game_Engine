#include "Runtime/Base/Transform.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Transform::Transform(void){
		mType = kTransformTypeNormal;
		mbAffectedByPhysics = false;
		mbDirty = true;
		mPosition.Set(0, 0, 0.0f);
		mRotation.Set(0, 0, 0);
		mLocalQuaternion.Set(0.0f, 0.0f, 0.0f, 1.0f);
		mScale.Set(1.0f, 1.0f, 1.0f);
	}
	void Transform::SetLocalPosition(float x, float y, float z, bool affectedbyphysics){
		mPosition.Set(x, y, z);
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
		mbAffectedByPhysics = affectedbyphysics;
	}

	void Transform::SetLocalRotation(float x, float y, float z, bool affectedbyphysics){
		mRotation.Set(x, y, z);
		mLocalQuaternion = EulerToQuaternion(mRotation * ALICE_PI_DIV_180);
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
		mbAffectedByPhysics = affectedbyphysics;
	}
	void Transform::SetLocalScale(float x, float y, float z){
		mScale.Set(x, y, z); 
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
	}
	void Transform::SetLocalShearing(float x, float y, float z) {
		mShearing.Set(x, y, z);
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
	}
	void Transform::ToMatrix(Matrix4x4&matrix) {
		Matrix3x3 rotationMatrix;
		if (mShearing.x == 0.0f&&mShearing.y == 0.0f) {
			QuaternionToMatrix(mLocalQuaternion, rotationMatrix);
		}
		else {
			Matrix3x3 shearing_matrix(1.0f);
			shearing_matrix.mData[1] = ALICE_DEG2RAD(mShearing.x);
			shearing_matrix.mData[3] = ALICE_DEG2RAD(-mShearing.y);
			QuaternionToMatrix(mLocalQuaternion, rotationMatrix);
			rotationMatrix = rotationMatrix * shearing_matrix;
		}
		matrix.LoadIdentity();
		matrix.SetTranslatePart(mPosition.x, mPosition.y, mPosition.z);
		matrix.SetRotationPart(rotationMatrix.mData);
		matrix.Scale(mScale.x, mScale.y, mScale.z);
	}
	void Transform::operator =(const Transform*r) {
		mPosition.Set(r->mPosition.v);
		mScale.Set(r->mScale.v);
		mRotation.Set(r->mRotation.v);
		mShearing.Set(r->mShearing.v);
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
	}
	void Transform::operator =(const Transform&r){
		mPosition.Set(r.mPosition.v);
		mScale.Set(r.mScale.v);
		mRotation.Set(r.mRotation.v);
		mShearing.Set(r.mShearing.v);
		mbDirty = true;
		if (mOwner != nullptr) {
			mOwner->MarkWorldMatrixDirty();
			mOwner->mbLocalMatrixDirty = true;
		}
	}
	void Transform::Dump(int level/* =0 */) {
		Info("local position : %f,%f,%f", mPosition.x, mPosition.y, mPosition.z);
		Info("local rotation : %f,%f,%f", mRotation.x, mRotation.y, mRotation.z);
		Info("local scale : %f,%f,%f", mScale.x, mScale.y, mScale.z);
		Info("local shearing : %f,%f,%f", mShearing.x, mShearing.y, mShearing.z);
	}
	int Transform::Export(lua_State*L){
		luaL_Reg api_m[] = {
			EXPORT_API(SetPosition)
			EXPORT_API(GetPosition)
			EXPORT_API(SetRotation)
			EXPORT_API(GetRotation)
			EXPORT_API(SetScale)
			EXPORT_API(GetScale)
			EXPORT_API(SetShearing)
			EXPORT_API(GetShearing)
			EXPORT_API_END
		};
		NEW_LUA_TYPE_WITH_API(L, Transform, api_m);
		SET_PARENT(L, Component);
		return 0;
	}

	int Transform::API_SetPosition(lua_State*L) {
		if (lua_gettop(L) != 4){
			errorC("Transform:SetPosition arg count must be 3,current is %d", lua_gettop(L)-1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)){
			errorC("Transform:SetPosition arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalPosition(x, y, z);
		return 0;
	}
	int Transform::API_GetPosition(lua_State*L) {
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->mPosition.x);
		lua_pushnumber(L, ptr->mPosition.y);
		lua_pushnumber(L, ptr->mPosition.z);
		return 3;
	}
	int Transform::API_SetRotation(lua_State*L) {
		if (lua_gettop(L) != 4) {
			errorC("Transform:SetRotation arg count must be 3,current is %d", lua_gettop(L) - 1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)) {
			errorC("Transform:SetRotation arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalRotation(x, y, z);
		return 0;
	}
	int Transform::API_GetRotation(lua_State*L) {
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->mRotation.x);
		lua_pushnumber(L, ptr->mRotation.y);
		lua_pushnumber(L, ptr->mRotation.z);
		return 3;
	}
	int Transform::API_SetScale(lua_State*L) {
		if (lua_gettop(L) != 4) {
			errorC("Transform:SetScale arg count must be 3,current is %d", lua_gettop(L) - 1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)) {
			errorC("Transform:SetScale arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalScale(x, y, z);
		return 0;
	}
	int Transform::API_GetScale(lua_State*L) {
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->mScale.x);
		lua_pushnumber(L, ptr->mScale.y);
		lua_pushnumber(L, ptr->mScale.z);
		return 3;
	}
	int Transform::API_SetShearing(lua_State*L) {
		if (lua_gettop(L) != 4) {
			errorC("Transform:SetShearing arg count must be 3,current is %d", lua_gettop(L) - 1);
			return 0;
		}
		if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4)) {
			errorC("Transform:SetShearing arg must be number,current 1(%s),2(%s),3(%s)", LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalShearing(x, y, z);
		return 0;
	}
	int Transform::API_GetShearing(lua_State*L) {
		Transform*ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->mShearing.x);
		lua_pushnumber(L, ptr->mShearing.y);
		lua_pushnumber(L, ptr->mShearing.z);
		return 3;
	}
}
