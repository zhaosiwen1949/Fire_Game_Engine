#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Math/Vector4.h"
#include "Runtime/Math/AliceMatrix4x4.h"
#include "Runtime/Base/Component.h"
namespace Alice {
	enum LightSourceType{
		kLightSourceTypeDirection = 1,
		kLightSourceTypePoint = 2,
		kLightSourceTypeSpot = 3
	};
	class Light :public Component {
	public:
		Light();
		virtual ~Light();
		Vector4f mPosition;
		Color4f mAmbientColor,mDiffuseColor,mSpecularColor;
		Vector4f mSetting,mSetting1;
		Matrix4x4 mProjection, mView;
		AliceUInt32 mCullingMask;
		LightSourceType mType;
		void Update(float deltaTime);
	public:
		static std::set<Light*> mLights;
		static int mLightEnableMask[8];
		static Vector4f mLightPos[8];
		static Vector4f mLightAmbient[8];
		static Vector4f mLightDiffuse[8];
		static Vector4f mLightSpecular[8];
		static Vector4f mLightSetting[8];
		static Vector4f mLightSetting1[8];
		static Light * mMainLight;
		DECLEAR_ALICE_CLASS(Light)
	};
}