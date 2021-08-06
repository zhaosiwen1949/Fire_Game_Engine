#include "SceneManager.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Utils/Timer.h"
#include "Runtime/Debugger/Log.h"
#include "Runtime/Debugger/Debugger.h"
#include "External/LibCurl/include/curl/curl.h"
#include "Runtime/Font/DynamicFont.h"
#include "GameObject.h"
#include "Runtime/2D/ImageSprite.h"
#include "Runtime/2D/ImageSprite9.h"
#include "Runtime/2D/Label.h"
#include "Runtime/2D/Canvas.h"
#include "Runtime/Animation/SpineAvatar.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/Render/DrawCall.h"

#include "Runtime/Device/DeviceInfo.h"
#include "Runtime/Physics/PhysicsManager.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "Runtime/Exported/Scene/ExportedGameObject.h"
#include "Runtime/2D/FullScreenQuad.h"
#include "Runtime/Thread/MainThreadScheduler.h"
#include "Runtime/Thread/AliceThread.h"

#include "Runtime/Exported/Render/ExportedMaterial.h"
extern "C" void InitMemory();
extern "C" void OnQuitMemory();
static Alice::SceneManager *sSceneManager;
static Alice::DrawCallInfo sRenderState;
namespace Alice {
	static std::mutex sMainThreadMutex;
	std::mutex & GetMainThreadMutex() {
		return sMainThreadMutex;
	}
	DrawCallInfo::DrawCallInfo() :mVertexCount(0), mTriangleCount(0), mDCCount(0), mCurrentFPS(0), mSchedurlerTime(0), mAnimationTime(0), mPhysicsTime(0),
		mFrameAvrageTimeCost(0), mLastFrameCPUTime(0), mLastFrameGPUTime(0), mLastFrameOverHeadTime(0), mLastFrameTotalTime(0), mTimer(nullptr),
		mLastFrameVSyncTime(0)
	{
		mTimer = new Alice::Timer;
		mTimer->Start();
	}

	void DrawCallInfo::ResetLastFrameInfo()
	{
		mLastFrameCPUTime = 0.0f;
		mLastFrameGPUTime = 0.0f;
		mLastFrameOverHeadTime = 0.0f;
	}
	//for editor end
	SceneManager::SceneManager() :mFullResolutionWidth(0), mFullResolutionHeight(0), mMaxFPS(0), mFixedTimePerFrame(0.0f), mSinCosTimeForBuiltinValues(0.0f) {
		sSceneManager = this;
		mbIsPlaying = false;
		mTimer.Start();
		mInternalOffScreenSceneFBO = nullptr;
		mInternalOffScreenSceneMaterial = nullptr;
		mFullScreenQuad = nullptr;
		mInternalOffScreenUIFBO = nullptr;
		mInternalOffScreenUIMaterial = nullptr;
		mSystemDefaultFBO = nullptr;
		mUIMaxScale = 1.0f;
		mOnException = LUA_REFNIL;
		mbEnableScenePostRendering = false;
		mbEnableUIPostRendering = false;
	}
	void SceneManager::Play() {
		mbIsPlaying = true;
	}
	void SceneManager::Step() {
	}
	void SceneManager::Pause(){
		mbIsPlaying = false;
	}
	void SceneManager::Reset(){
	}
	void SceneManager::Stop() {
		mbIsPlaying = false;
	}
	void SceneManager::AddCamera(Camera*camera) {
		mCameras.push_back(camera);
	}
	void SceneManager::RemoveCamera(Camera*camera) {
		auto iter = mCameras.begin();
		auto iterEnd = mCameras.end();
		for (; iter != iterEnd; ++iter) {
			if (*iter == camera) {
				mCameras.erase(iter);
				break;
			}
		}
	}
	void SceneManager::AddOffScreenCamera(Camera*camera) {
		mOffScreenCameras.push_back(camera);
	}
	void SceneManager::RemoveOffScreenCamera(Camera*camera) {
		auto iter = mOffScreenCameras.begin();
		auto iterEnd = mOffScreenCameras.end();
		for (; iter != iterEnd; ++iter) {
			if (*iter == camera) {
				mOffScreenCameras.erase(iter);
				break;
			}
		}
	}
	void SceneManager::Update(DrawCallInfo &rs) {
		mTimer.Reset();
		if (mSystemDefaultFBO == nullptr) {
			GLint current_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);
			mSystemDefaultFBO = new FBO(current_fbo);
			mSystemDefaultFBO->Resize(mFullResolutionWidth, mFullResolutionHeight);
			mSystemDefaultFBO->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		float deltaTime = rs.mLastFrameTotalTime / 1000.0f;
		if (mbIsPlaying) {
			mTimeSinceStartUp += deltaTime;
			mSinCosTimeForBuiltinValues += deltaTime;
			if (GetPhysicsManager().FixedUpdate(deltaTime)) {
				//mRootObject->FixedSyncPhysicsToLogicalRecusively(deltaTime);
			}
			MainThreadScheduler::Update(deltaTime);
			AliceThread::UpdateAliceThreads();
			if (mMaxFPS != 0) {
				Render(rs);
				rs.mLastFrameCPUTime = mTimer.GetTimeInMs();
				rs.mLastFrameSleepTime = mFixedTimePerFrame - rs.mLastFrameCPUTime;
				if (rs.mLastFrameSleepTime > 0) {
					AliceSleep(int(rs.mLastFrameSleepTime));
				}
				else {
					rs.mLastFrameSleepTime = 0.0f;
				}
			}
			else {
				Render(rs);
				rs.mLastFrameCPUTime = mTimer.GetTimeInMs();
				rs.mLastFrameSleepTime = 0.0f;
			}
		}
		else {
			Render(rs);
			rs.mLastFrameCPUTime = mTimer.GetTimeInMs();
			rs.mLastFrameSleepTime = 0.0f;
		}
	}
	void SceneManager::Render(DrawCallInfo&rs) {
		float deltaTime = rs.mLastFrameTotalTime / 1000.0f;
		if (mSinCosTimeForBuiltinValues > 6.283185f) {
			mSinCosTimeForBuiltinValues = 0.0f;
		}
		Material::mTime.mValue.Set(mTimeSinceStartUp / 20.0f, mTimeSinceStartUp, mTimeSinceStartUp*2.0f, mTimeSinceStartUp*3.0f);
		Material::mSinTime.mValue.Set(sinf(mSinCosTimeForBuiltinValues / 2.0f), sinf(mSinCosTimeForBuiltinValues), sinf(mSinCosTimeForBuiltinValues*2.0f), sinf(mSinCosTimeForBuiltinValues*4.0f));
		Material::mCosTime.mValue.Set(cosf(mSinCosTimeForBuiltinValues / 2.0f), cosf(mSinCosTimeForBuiltinValues), cosf(mSinCosTimeForBuiltinValues*2.0f), cosf(mSinCosTimeForBuiltinValues*4.0f));

		mRootObject->UpdateScriptsRecursively(deltaTime);//update script、this may change transform/component
		mRootObject->UpdateTransformRecursively();//update transform
		mRootObject->UpdateComponentRecusively(deltaTime);//update component
		mRootObject->SyncPhysicsComponentRecusively(deltaTime);//sync rendering geometry to physic world
		//然后渲染其他的摄像机
		if (mCameras.empty()&&Canvas::mCanvases.empty()) {
			return;
		}
#if ALICE_EDITOR
		rs.mVertexCount = 0;
		rs.mDCCount = 0;
		rs.mTriangleCount = 0;
#endif
		//rendering offscreen camera
		RenderOffScreenCamera(rs);
		RenderScene(rs);
		RenderUI(rs);
	}

	void SceneManager::RenderOffScreenCamera(DrawCallInfo &rs) {
		std::sort(mOffScreenCameras.begin(), mOffScreenCameras.end(), Camera::SortCameras);
		auto iterStart = mOffScreenCameras.begin();
		auto iterEnd = mOffScreenCameras.end();
		for (auto iter = iterStart; iter != iterEnd; ++iter) {
			RenderDepthCamera(*iter
#if ALICE_EDITOR
				, rs
#endif
			);
		}
		for (auto iter = iterStart; iter != iterEnd; ++iter) {
			RenderCamera(*iter
#if ALICE_EDITOR
				, rs
#endif
			);
		}
	}
	void SceneManager::RenderScene(DrawCallInfo &rs) {
		if (mbEnableScenePostRendering) {
			mInternalOffScreenSceneFBO->Active();
		}
		else {
			mSystemDefaultFBO->Active();
		}
		std::sort(mCameras.begin(), mCameras.end(), Camera::SortCameras);
		//rendering depth camera
		for (auto iter = mCameras.begin(); iter != mCameras.end(); iter++) {
			RenderDepthCamera(*iter
#if ALICE_EDITOR
				, rs
#endif
			);
		}
		//rendering visiable camera
		//render scene camera
		for (auto iter = mCameras.begin(); iter != mCameras.end(); iter++) {
			if ((*iter)->mCameraMode == Camera::CameraMode::PerspectiveMode) {
				//Info("render camera -- %s",(*iter)->mOwner->mName.mText);
				RenderCamera(*iter
#if ALICE_EDITOR
					, rs
#endif
				);
			}
		}
		if (mbEnableScenePostRendering) {
			if (mbEnableUIPostRendering == false) {
				mSystemDefaultFBO->Active();
				mFullScreenQuad->Render(mInternalOffScreenSceneMaterial);
			}
		}
	}
	void SceneManager::RenderUI(DrawCallInfo &rs) {
		if (mbEnableUIPostRendering) {
			mInternalOffScreenUIFBO->Active();
			//draw scene image first
			if (mbEnableScenePostRendering) {
				mFullScreenQuad->Render(mInternalOffScreenSceneMaterial);
			}
		}
		//render ui camera
		for (auto iter = mCameras.begin(); iter != mCameras.end(); iter++) {
			if ((*iter)->mCameraMode == Camera::CameraMode::OrthoMode) {
				RenderCamera(*iter
#if ALICE_EDITOR
					, rs
#endif
				);
			}
		}
		//render canvas
		for (auto iter = Canvas::mCanvases.begin(); iter != Canvas::mCanvases.end(); iter++) {
			Canvas*current_canvas = *iter;
			RenderQueue render_queue(current_canvas);
			current_canvas->mOwner->RenderUIComponentRecursively(&render_queue
#if ALICE_EDITOR
				, rs
#endif
			,false);
			render_queue.RenderUIEx();
		}
		if (mbEnableUIPostRendering) {
			mSystemDefaultFBO->Active();
			mFullScreenQuad->Render(mInternalOffScreenUIMaterial);
		}
	}
	void SceneManager::RenderCamera(Camera*camera
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
		if (camera->IsEnabled()) {
			if (camera->mbOffScreen) {
				camera->mFBO->Active();
			}
			camera->ClearBackground();
			/*
			clip divide : clip -> ndc (-1~1)
			projection matrix : view->clip
			view matrix : world -> view
			model matrix : model -> world
			*/
			if (camera->mCameraMode == Camera::CameraMode::PerspectiveMode) {
				Matrix4x4 clipToWorld = camera->mProjectionMatrix;
				clipToWorld *= camera->mViewMatrix;//world->clip
				ExtractProjectionPlanes(clipToWorld, camera->mCullingParameters.cullingPlanes);
			}
			RenderQueue render_queue(camera);
			mRootObject->RenderComponentRecursively(&render_queue
#if ALICE_EDITOR
				, rs
#endif
			);
			render_queue.RenderOpaque();
			render_queue.RenderTransparent();
			render_queue.RenderUI();
			if (camera->mbOffScreen) {
				camera->mFBO->Restore();
				camera->mOwner->PostRendering(camera);
			}
		}
	}
	void SceneManager::RenderDepthCamera(Camera*camera
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
		if (camera->mbRenderDepth&&camera->IsEnabled()) {
			camera->mDepthFBO->ActiveDepth();
			RenderQueue render_queue(camera);
			mRootObject->RenderShadowMapRecursively(camera, &render_queue
#if ALICE_EDITOR
				, rs
#endif
			);
			render_queue.RenderDepth();
			camera->mDepthFBO->Restore();
		}
	}
	void SceneManager::GetCanvasSize(int&width, int&height) {
		width = mFullResolutionWidth;
		height = mFullResolutionHeight;
	}
	void SceneManager::OnViewSizeChange(int x, int y, int width, int height) {
		if (mFullResolutionWidth == width && mFullResolutionHeight == height) {
			return;
		}
		mFullResolutionWidth = width;
		mFullResolutionHeight = height;
		float device_scale = DeviceInfo::GetScale();
		int fixed_width = mFullResolutionWidth;
		int fixed_height = mFullResolutionHeight;
		GetCanvasSize(fixed_width, fixed_height);
		Canvas::OnViewportChanged(fixed_width, fixed_height);
	}
	void SceneManager::Init() {
		lua_State*L = Alice::LuaEngine::Store();
		Alice::ExportedGameObject::New_ExportedAliceType(L);
		mRootObject = TO_USERDATA(L, Alice::ExportedGameObject, -1);
		mRootObject->mLayer = 0xFFFFFFFF;
		mRootObject->mName = "InternalRoot";
		Alice::LuaEngine::Restore();
		mFullScreenQuad = new FullScreenQuad;
		mFullScreenQuad->Init();
	}
	void SceneManager::EnableScenePostRendering(bool enable, Material*material) {
		mbEnableScenePostRendering = enable;
		if (mInternalOffScreenSceneFBO == nullptr) {
			float device_scale = DeviceInfo::GetScale();
			int fixed_width = mFullResolutionWidth;
			int fixed_height = mFullResolutionHeight;
			GetCanvasSize(fixed_width, fixed_height);
			if (material == nullptr) {
				material = ExportedMaterial::Clone("builtin/DefaultFrameBufferRender");
			}
			mInternalOffScreenSceneMaterial = material;
			RecreateInternalFBO(&mInternalOffScreenSceneFBO, mInternalOffScreenSceneMaterial, fixed_width, fixed_height);
		}
	}

	void SceneManager::EnableUIPostRendering(bool enable, Material*material) {
		mbEnableUIPostRendering = enable;
		if (mInternalOffScreenUIFBO == nullptr) {
			float device_scale = DeviceInfo::GetScale();
			int fixed_width = mFullResolutionWidth;
			int fixed_height = mFullResolutionHeight;
			GetCanvasSize(fixed_width, fixed_height);
			if (material == nullptr) {
				material = ExportedMaterial::Clone("builtin/DefaultFrameBufferRender");
			}
			mInternalOffScreenUIMaterial = material;
			RecreateInternalFBO(&mInternalOffScreenUIFBO, mInternalOffScreenUIMaterial, fixed_width, fixed_height);
			mInternalOffScreenUIFBO->SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	void SceneManager::RecreateInternalFBO(FBO**ppfbo, Material*material, int width, int height) {
		if (*ppfbo != nullptr) {
			delete (*ppfbo);
		}
		*ppfbo = new FBO;
		(*ppfbo)->Init(width, height);
		material->SetTextureProperty("U_Texture", (*ppfbo)->mColorBuffer.mPtr);
		material->SetTextureProperty("Alice_FrameBuffer", (*ppfbo)->mColorBuffer.mPtr);
		Info("internal fbo size %dx%d full resolution %dx%d", width, height, mFullResolutionWidth, mFullResolutionHeight);
	}
}
//1、init log
//2、init first system info
//3、init memory pool
//4、init lua engine & export alice core runtime lua api
//5、init physics engine
//6、init image error reporter
//7、init default engine asset bundle map
//8、init physics engine
extern "C" void InitEngineCore() {
	curl_global_init(CURL_GLOBAL_ALL);
	InitMemory();
	Alice::Resource::Init();
	Alice::ResourceManager::Init();
	Alice::FixedString temp(256);
	temp = *Alice::ResourceManager::mDocumentDir;
	temp << "Log/EngineCoreLog";
	InitEngineLog(temp.mText);
	Alice::LuaEngine::Init();
	GetPhysicsManager().Init();
	Alice::ImageUtils::Init();
	Info("curl version %s", curl_version());
}
//9、init font engine
//10、init builtin resources
//11、init default scene root object
//12、init builtin scripts
//13、init graphic engine for post rendering
//should be inited after opengl setup
extern "C" void InitRuntime() {
	Alice::GlobalRenderState::Init();
	Alice::DynamicFont::InitFTLib();
	GetSceneManager()->Init();
	//V_Color
	Alice::GetRendererManager().InitDefaultColorMaterial();
	//U_Color
	Alice::GetRendererManager().InitDefaultSimpleColorMaterial();
	//sprite 2d material
	Alice::GetRendererManager().InitUIMaterial();
	Alice::GetRendererManager().InitStencilUIMaterial();
	Alice::GetRendererManager().InitDefault2DMaterial();
	//dynamic font
	Alice::GetRendererManager().InitDynamicFontMaterial();
	Alice::GetRendererManager().InitDefaultFrameBufferRenderMaterial();
	Alice::GetRendererManager().InitStencilRenderMaterial();
}
extern "C" void OnQuitEngine() {
	OnQuitMemory();
}
extern "C" void Render() {
	std::lock_guard<std::mutex> lock(Alice::GetMainThreadMutex());
	GetSceneManager()->Update(sRenderState);
	sRenderState.mLastFrameTotalTime = sRenderState.mLastFrameSleepTime + sRenderState.mLastFrameCPUTime;
	sRenderState.mLastFrameCPUTime = 0.0f;
	sRenderState.mLastFrameGPUTime = 0.0f;
	sRenderState.mLastFrameOverHeadTime = 0.0f;
}
extern "C" void OnViewSizeChange(int x, int y, int width, int height) {
	GetSceneManager()->OnViewSizeChange(x, y, width, height);
}
Alice::SceneManager * GetSceneManager() {
	if (sSceneManager == nullptr) {
		sSceneManager = new Alice::SceneManager();
	}
	return sSceneManager;
}
