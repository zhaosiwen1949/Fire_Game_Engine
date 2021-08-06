#include "Light.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice {
	Light*Light::mMainLight = nullptr;
	std::set<Light*> Light::mLights;
	int Light::mLightEnableMask[8];
	Vector4f Light::mLightPos[8];
	Vector4f Light::mLightAmbient[8];
	Vector4f Light::mLightDiffuse[8];
	Vector4f Light::mLightSpecular[8];
	Vector4f Light::mLightSetting[8];
	Vector4f Light::mLightSetting1[8];
	Light::Light() {
		mType = kLightSourceTypeDirection;
		mPosition.Set(0.0f, 1.0f, 0.0f,0.0f);
		mAmbientColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		mDiffuseColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		mSpecularColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Light::~Light() {
		auto iter = mLights.find(this);
		if (iter != mLights.end()) {
			mLights.erase(iter);
		}
	}

	void Light::Update(float deltaTime) {
		if (mbPositionChanged) {
			mbPositionChanged = false;
			switch (mType)
			{
			case Alice::kLightSourceTypeDirection:
				mPosition.x = mOwner->mLocalTransform.mPosition.x;
				mPosition.y = mOwner->mLocalTransform.mPosition.y;
				mPosition.z = mOwner->mLocalTransform.mPosition.z;
				mPosition.w = 0.0f;
				break;
			case Alice::kLightSourceTypePoint:
			{
				const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
				mPosition.x = world_matrix.mData[12];
				mPosition.y = world_matrix.mData[13];
				mPosition.z = world_matrix.mData[14];
				mPosition.w = 1.0f;
			}
				break;
			case Alice::kLightSourceTypeSpot:
			{
				const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
				mPosition.x = world_matrix.mData[12];
				mPosition.y = world_matrix.mData[13];
				mPosition.z = world_matrix.mData[14];
			}
				break;
			default:
				break;
			}
		}
	}
}