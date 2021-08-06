#pragma once
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Utils/LinkedList.h"
#include "AliceGL.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Render/GL20/EBO.h"
namespace Alice{
	class Camera;
	class Canvas;
	class CustomComponent;
	class RenderPass;
	class GameObject;
	class Material;
	class Light;
	class FrontToBackDoubleLinkedList :public DoubleLinkedList {
	public:
		float mDistanceToCamera;
	};
	class DrawCall:public FrontToBackDoubleLinkedList {
	public:
		Material *mMaterial;
		Camera*mCamera;
		Canvas*mCanvas;
		VBO *mVBO;
		EBO *mEBO;
        GameObject*mGameObject;
		CustomComponent*mCustomComponent;
	public:
		DrawCall();
		~DrawCall();
		void Clear();
		void Rendering();
		void RenderingOpaque();
		void RenderingTransparent();
		void SetUpDepthMap(RenderPass*pass);
		void SetUpCamera(RenderPass*pass);
		void SetUpOnePassLights(RenderPass*pass);
		void SetUpLight(RenderPass*pass,Light*light);
		void RenderNoLit(RenderPass*pass);
		void RenderUI(RenderPass*pass);
		void RenderOnePass(RenderPass*pass);
		void RenderForwardBasePass(RenderPass*pass);
		void RenderForwardAdditivePass(RenderPass*pass);
		void ForwardRendering();
		void RenderingDepth();
	};
	class RenderQueue {
	public:
		Camera*mCamera;
		Canvas*mCanvas;
		RenderQueue(Camera*camera){
			mCamera = camera;
			mCanvas = nullptr;
			mOpaqueDrawCall = nullptr;
			mTransparentDrawCall = nullptr;
			mLastTransparentDrawCall = nullptr;
			mUIDrawCall = nullptr;
			mDepthDrawCall = nullptr;
			mLastUIDrawCall = nullptr;
		}
		RenderQueue(Canvas*canvas) {
			mCamera = nullptr;
			mCanvas = canvas;
			mOpaqueDrawCall = nullptr;
			mTransparentDrawCall = nullptr;
			mLastTransparentDrawCall = nullptr;
			mUIDrawCall = nullptr;
			mDepthDrawCall = nullptr;
			mLastUIDrawCall = nullptr;
		}
		DrawCall *mOpaqueDrawCall, *mTransparentDrawCall,*mLastTransparentDrawCall, *mUIDrawCall,*mLastUIDrawCall, *mDepthDrawCall;
		void AppendOpaqueDrawCall(DrawCall*dc);
		void AppendTransparentDrawCall(DrawCall*dc);
		void AppendUIDrawCall(DrawCall*dc);
		void AppendDepthDrawCall(DrawCall*dc);
		void RenderOpaque();
		void RenderTransparent();
		void RenderUI();
		void RenderUIEx();
		void RenderDepth();
	};
}
