#pragma once
#include "Runtime/Render/Camera.h"

namespace Alice
{
	class ExportedCamera :public Camera
	{
	public:
		static int Export(lua_State*L);
		DEFINE_LUA_API(GetCameraMode);
		DEFINE_LUA_API(GetClearMode);
		DEFINE_LUA_API(GetDepth);
		DEFINE_LUA_API(GetRenderTexture);
		DEFINE_LUA_API(GetDepthTexture);
		DEFINE_LUA_API(GetClearColor);
		DEFINE_LUA_API(GetClipNearFar);
		DEFINE_LUA_API(GetResolution);

		DEFINE_LUA_API(SetCameraMode);
		DEFINE_LUA_API(SetClearMode);
		DEFINE_LUA_API(SetSkyBox);
		DEFINE_LUA_API(GetSkyBoxCubeMap);
		DEFINE_LUA_API(MakeOffScreen);
		DEFINE_LUA_API(SetDepthMaterial);
		DEFINE_LUA_API(RenderDepth);
		DEFINE_LUA_API(SetDepth);
		DEFINE_LUA_API(SetWidth);
		DEFINE_LUA_API(SetRenderTexture);
		DEFINE_LUA_API(SetClearColor);
		DEFINE_LUA_API(SetClipNearFar);
		DEFINE_LUA_API(SetFOV);
		DEFINE_LUA_API(SetCullingMask);
		DEFINE_LUA_API(SetViewport);
		DEFINE_LUA_API(SystemCoordToCameraCoord);
		DEFINE_LUA_API(GetRay);
		DEFINE_LUA_API(CastRay);
		DEFINE_LUA_API(GetDirection);
		DEFINE_LUA_API(GetRight);
		DEFINE_LUA_API(GetUp);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedCamera, Camera)
	};
}
