#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/Camera.h"
namespace Alice
{
	struct RenderingParam
	{
		float*mViewMatrix;
		float*mProjectionMatrix;
	};
	class CustomComponent :public Component
	{
	public:
		void Update(float deltaTime);
		void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		void Rendering(Camera*camera);
		CustomComponent();
		virtual ~CustomComponent();
	public:
		SmartPtr<Material> mMaterial;
		int mRenderingParam;
		int mUpdater;
		int mRenderer;
	public:
		DECLEAR_ALICE_CLASS(CustomComponent)
	};
}