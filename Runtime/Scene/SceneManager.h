#pragma once
#include <vector>
#include <mutex>
#include "Runtime/Utils/Timer.h"
namespace Alice {
	class DrawCallInfo {
	public:
		int mVertexCount;
		int mTriangleCount;
		int mDCCount;
		int mCurrentFPS;

		float mSchedurlerTime;
		float mAnimationTime;
		float mPhysicsTime;

		Timer *mTimer;
		float mLastFrameOverHeadTime;
		float mLastFrameCPUTime;
		float mLastFrameGPUTime;
		float mLastFrameTotalTime;
		float mLastFrameSleepTime;
		float mFrameAvrageTimeCost;
		float mLastFrameVSyncTime;

		DrawCallInfo();
		void ResetLastFrameInfo();
	};
	class Camera;
	class GameObject;
	class FBO;
	class Material;
	class FullScreenQuad;
	class Timer;
	class SceneManager {
	public:
		SceneManager();
		void Init();
		void Update(DrawCallInfo &rs);
		void Render(DrawCallInfo &rs);
		void GetCanvasSize(int&width, int&height);
		void OnViewSizeChange(int x, int y, int width, int height);
		void Pause();
		void Play();
		void Step();
		void Reset();
		void Stop();
		void AddOffScreenCamera(Camera*camera);
		void RemoveOffScreenCamera(Camera*camera);
		void AddCamera(Camera*camera);
		void RemoveCamera(Camera*camera);
		void RenderOffScreenCamera(DrawCallInfo &rs);
		void RenderScene(DrawCallInfo &rs);
		void RenderUI(DrawCallInfo &rs);
		void RenderCamera(Camera*camera
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		void RenderDepthCamera(Camera*camera
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		int mFullResolutionWidth, mFullResolutionHeight;
		int mMaxFPS, mOnException;
		float mFixedTimePerFrame, mTimeSinceStartUp;
		float mSinCosTimeForBuiltinValues;
		float mUIMaxScale;
		bool mbIsPlaying;
		Timer mTimer;
		GameObject	*mRootObject;
		std::vector<Camera*> mOffScreenCameras;
		std::vector<Camera*> mCameras;
		bool mbEnableScenePostRendering, mbEnableUIPostRendering;
		FBO * mInternalOffScreenSceneFBO, *mInternalOffScreenUIFBO, *mSystemDefaultFBO;
		Material*mInternalOffScreenSceneMaterial, *mInternalOffScreenUIMaterial;
		FullScreenQuad*mFullScreenQuad;
		void EnableScenePostRendering(bool enable = false, Material*material = nullptr);
		void EnableUIPostRendering(bool enable = false, Material*material = nullptr);
		void RecreateInternalFBO(FBO**fbo, Material*material, int width, int height);
	};
	std::mutex & GetMainThreadMutex();
}
Alice::SceneManager * GetSceneManager();