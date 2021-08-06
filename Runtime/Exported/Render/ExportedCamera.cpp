#include "ExportedCamera.h"
#include "Runtime/Scene/SceneManager.h"
#include "ExportedMaterial.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Exported/Geometry/ExportedRay.h"
#include "Runtime/Exported/Scene/ExportedGameObject.h"
#include "Runtime/Exported/Render/ExportedCamera.h"
#include "Runtime/Exported/Render/ExportedTexture2D.h"
#include "Runtime/Exported/Render/ExportedTextureCube.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/Builtin/3D/SkyBox.h"
#include "Runtime/Physics/PhysicsManager.h"
namespace Alice
{
	int ExportedCamera::Export(lua_State*L)
	{
		luaL_Reg api[] = {
			EXPORT_API(GetCameraMode)
			EXPORT_API(GetClearMode)
			EXPORT_API(GetDepth)
			EXPORT_API(GetRenderTexture)
			EXPORT_API(GetDepthTexture)
			EXPORT_API(GetClearColor)
			EXPORT_API(GetClipNearFar)
			EXPORT_API(GetResolution)
			EXPORT_API(GetDirection)
			EXPORT_API(GetRight)
			EXPORT_API(GetUp)

			EXPORT_API(SetCameraMode)
			EXPORT_API(SetClearMode)
			EXPORT_API(SetSkyBox)
			EXPORT_API(GetSkyBoxCubeMap)
			EXPORT_API(MakeOffScreen)
			EXPORT_API(SetDepthMaterial)
			EXPORT_API(RenderDepth)
			EXPORT_API(SetDepth)
			EXPORT_API(SetWidth)
			EXPORT_API(SetRenderTexture)
			EXPORT_API(SetClearColor)
			EXPORT_API(SetClipNearFar)
			EXPORT_API(SetFOV)
			EXPORT_API(SetCullingMask)
			EXPORT_API(SetViewport)
			EXPORT_API(SystemCoordToCameraCoord)
			EXPORT_API(GetRay)
			EXPORT_API(CastRay)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Camera, api);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = {EXPORT_API(ClassID) { NULL,NULL } };
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Camera, api_c);
		/*
		kClearModeSkybox = 1,
		kClearModeSolidColor = 2,
		kClearModeDepthOnly = 3,
		kClearModeDontClear = 4,
		kClearModeClearAll=5
		*/
		lua_pushnumber(L, kClearModeSkybox);
		lua_setfield(L, -2, "ClearSkyBox");
		lua_pushnumber(L, kClearModeSolidColor);
		lua_setfield(L, -2, "ClearSolidColor");
		lua_pushnumber(L, kClearModeDepthOnly);
		lua_setfield(L, -2, "ClearDepth");
		lua_pushnumber(L, kClearModeClearAll);
		lua_setfield(L, -2, "ClearAll");//clear all buffer bit
		lua_pushnumber(L, kClearModeDontClear);
		lua_setfield(L, -2, "DontClear");
		lua_pushnumber(L, PerspectiveMode);
		lua_setfield(L, -2, "PerspectiveMode");
		lua_pushnumber(L, OrthoMode);
		lua_setfield(L, -2, "OrthoMode");
		return 0;
	}

	int ExportedCamera::API_CastRay(lua_State*L) {
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		float mouse_x = lua_tonumber(L, 2);
		float mouse_y = lua_tonumber(L, 3);
		mouse_x += ptr->mViewWidth / 2.0f;
		mouse_y = ptr->mViewHeight / 2.0f - mouse_y;
		Debug("mouse pos %f,%f", mouse_x, mouse_y);
		float ndc_x = (float)mouse_x*2.0f / (float)ptr->mViewWidth - 1.0f;
		float ndc_y = 1.0f - (float)mouse_y*2.0f / (float)ptr->mViewHeight;
		Vector4f ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);
		Matrix4x4 inverse_projection_matrix, inverse_view_matrix;
		InvertMatrix4x4(ptr->mProjectionMatrix.mData, inverse_projection_matrix.mData);
		Vector3f ray_eye = inverse_projection_matrix * ray_clip;
		InvertMatrix4x4(ptr->mViewMatrix.mData, inverse_view_matrix.mData);
		Vector3f ray_world = inverse_view_matrix * Vector4f(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
		ray_world.Normalize();
		const Matrix4x4&world_matrix = ptr->mOwner->GetWorldMatrix();
		Vector3f origin(world_matrix.mData[12], world_matrix.mData[13], world_matrix.mData[14]);
		Debug("ray info:\nfrom : %f,%f,%f\ndirection : %f,%f,%f", origin.x, origin.y, origin.z,
														ray_world.x, ray_world.y, ray_world.z);
		Ray ray(origin, ray_world);
		GameObject * root = GetSceneManager()->mRootObject;
		GameObject * go = nullptr;
		if (root->mRightSibling != nullptr) {
			go = root->RightSibling<GameObject>()->RayCastTestRecursively(&ray);
		}
		else {
			go = root->RayCastTestRecursively(&ray);
		}
		if (go != nullptr) {
			go->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedCamera::API_GetRay(lua_State*L){
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		float mouse_x = lua_tonumber(L, 2);
		float mouse_y = lua_tonumber(L, 3);
		float scale = ptr->mViewWidth / (ptr->mOrthoModeSize);
		mouse_x *= scale;
		mouse_y *= scale;
		mouse_x += ptr->mViewWidth / 2.0f;
		mouse_y = ptr->mViewHeight / 2.0f - mouse_y;
		Debug("mouse pos %f,%f",mouse_x,mouse_y);
		float ndc_x = (float)mouse_x*2.0f / (float)ptr->mViewWidth - 1.0f;
		float ndc_y = 1.0f - (float)mouse_y*2.0f / (float)ptr->mViewHeight;
		Debug("ndc x,y %f,%f\n", ndc_x, ndc_y);
		Vector3f ray_clip(ndc_x, ndc_y, -1.0f);
		Vector3f ray_eye=ptr->mProjectionMatrix.InverseMultiplyPoint3Affine(ray_clip);
		ray_eye.z = -1.0f;
		Vector3f ray_world = ptr->mViewMatrix.InverseMultiplyVector3Affine(ray_eye);
		ray_world.Normalize();
		Debug("world ray x,y,z %f,%f,%f\n", ray_world.x, ray_world.y, ray_world.z);
		lua_pushinteger(L, (int)ptr->mClearFlags);
		return 1;
	}

	int ExportedCamera::API_GetCameraMode(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushinteger(L, (int)ptr->mCameraMode);
		return 1;
	}

	int ExportedCamera::API_GetClearMode(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushinteger(L, (int)ptr->mClearFlags);
		return 1;
	}

	int ExportedCamera::API_GetDepth(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushnumber(L, ptr->mDepth);
		return 1;
	}

	int ExportedCamera::API_GetRenderTexture(lua_State*L){
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		if (ptr->mFBO!=nullptr&&ptr->mFBO->mColorBuffer!=nullptr){
			ptr->mFBO->mColorBuffer->PushUserData(L);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedCamera::API_GetDepthTexture(lua_State*L) {
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		if (ptr->mDepthFBO != nullptr&&ptr->mDepthFBO->mColorBuffer != nullptr) {
			ptr->mDepthFBO->mColorBuffer->PushUserData(L);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedCamera::API_GetClearColor(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushinteger(L, ptr->mClearColor.r);
		lua_pushinteger(L, ptr->mClearColor.g);
		lua_pushinteger(L, ptr->mClearColor.b);
		lua_pushinteger(L, ptr->mClearColor.a);
		return 4;
	}

	int ExportedCamera::API_GetClipNearFar(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushnumber(L, ptr->mNearClip);
		lua_pushnumber(L, ptr->mFarClip);
		return 2;
	}

	int ExportedCamera::API_GetResolution(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -1);
		lua_pushnumber(L, ptr->mViewWidth);
		lua_pushnumber(L, ptr->mViewHeight);
		return 2;
	}


	//setters
	int ExportedCamera::API_SetCameraMode(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->mCameraMode = lua_tointeger(L, 2) == 1 ? PerspectiveMode : OrthoMode;
		ptr->OnViewPortSizeChange(ptr->mViewWidth, ptr->mViewHeight);
		return 0;
	}

	int ExportedCamera::API_SetClearMode(lua_State*L)
	{
		if (!lua_isnumber(L, -1)){
			errorC("Camera:SetClearMode arg at index -1 must be a integer");
			return 0;
		}
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -2);
		ptr->mClearFlags = (ClearMode)lua_tointeger(L, -1);
		return 0;
	}

	int ExportedCamera::API_SetSkyBox(lua_State*L){
		int nArgc = lua_gettop(L);
		if (nArgc == 2) {//set texture cube
			ExportedCamera*camera = TO_USERDATA(L, ExportedCamera, 1);
			if (camera->mSkyBox != nullptr) {
				const char * texturePath = lua_tostring(L, 2);
				if (camera->mSkyBox->mCubeMap->mName==texturePath){//same texture
					return 0;
				}
				TextureCube*textureCube = TextureCube::LoadTextureCube(texturePath);
				if (textureCube != nullptr) {
					CreateLuaDataViaExistObject(textureCube, TextureCube, L);
					camera->mSkyBox->mCubeMap = textureCube;
					camera->mSkyBox->mMaterial->SetTextureCubeProperty(UniformDefines::Sampler_SkyBox_NameInShader, textureCube);
				}
			}

		}else if(nArgc==3){
			ExportedCamera*camera = TO_USERDATA(L, ExportedCamera, 1);
			ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 2);
			const char * texturePath = lua_tostring(L, 3);
			TextureCube*textureCube = TextureCube::LoadTextureCube(texturePath);
			if (textureCube != nullptr) {
				CreateLuaDataViaExistObject(textureCube, TextureCube, L);
				SkyBox *skyBox = new SkyBox;
				camera->mSkyBox = skyBox;
				skyBox->mCamera = camera;
				skyBox->mMaterial = mat;
				skyBox->mCubeMap = textureCube;
				mat->SetTextureCubeProperty(UniformDefines::Sampler_SkyBox_NameInShader, textureCube);
			}
		}
		return 0;
	}

	int ExportedCamera::API_GetSkyBoxCubeMap(lua_State*L) {
		ExportedCamera*camera = TO_USERDATA(L, ExportedCamera, 1);
		if (camera->mSkyBox != nullptr) {
			camera->mSkyBox->mCubeMap->PushUserData(L);
		}else{
			lua_pushnil(L);
		}
		return 1;
	}
	int ExportedCamera::API_MakeOffScreen(lua_State*L){
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		if (lua_toboolean(L,2)){//make this camera as offscreen camera
			if (!ptr->mbOffScreen) {
				ptr->mbOffScreen = true;
				GetSceneManager()->RemoveCamera(ptr);
				GetSceneManager()->AddOffScreenCamera(ptr);
				if (ptr->mFBO==nullptr){
					ptr->mFBO = new FBO;
					ptr->mFBO->Init(ptr->mViewWidth, ptr->mViewHeight);
				}
			}
		}else{
			if (ptr->mbOffScreen) {
				ptr->mbOffScreen = false;
				GetSceneManager()->RemoveOffScreenCamera(ptr);
				GetSceneManager()->AddCamera(ptr);
			}
		}
		return 0;
	}
	int ExportedCamera::API_SetDepthMaterial(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ExportedMaterial*mat = TO_USERDATA(L, ExportedMaterial, 2);
		ptr->mDepthMaterial = mat;
		return 0;
	}
	int ExportedCamera::API_RenderDepth(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		if (lua_toboolean(L, 2)) {
			if (false == ptr->mbRenderDepth) {
				ptr->mbRenderDepth = true;
				if (ptr->mDepthFBO == nullptr) {
					ptr->mDepthFBO = new FBO;
					if (lua_gettop(L)==3){
						if (lua_tointeger(L,3)==1){//create depth buffer for opengles,depth buffer is rbo,color buffer for depth data
							ptr->mDepthFBO->InitDepthFBOES(ptr->mViewWidth, ptr->mViewHeight);
						}
					}else{
						ptr->mDepthFBO->InitDepthFBO(ptr->mViewWidth, ptr->mViewHeight);
					}
				}
			}
		}else{
			ptr->mbRenderDepth = false;
		}
		return 0;
	}

	int ExportedCamera::API_SetDepth(lua_State*L)
	{
		if (!lua_isnumber(L, -1)){
			errorC("Camera:SetDepth arg at index -1 must be a number");
			return 0;
		}
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, -2);
		ptr->mDepth = lua_tonumber(L, -1);
		return 0;
	}

	int ExportedCamera::API_SetWidth(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->mOrthoModeSize = lua_tonumber(L, 2);
		ptr->OnViewPortSizeChange(ptr->mViewWidth, ptr->mViewHeight);
		return 0;
	}

	int ExportedCamera::API_SetViewport(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->OnViewPortSizeChange(lua_tointeger(L,2), lua_tointeger(L, 3));
		return 0;
	}

	int ExportedCamera::API_SystemCoordToCameraCoord(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		float ox = lua_tointeger(L, 2);
		float oy = lua_tointeger(L, 3);

		float scale = ptr->mViewWidth / (ptr->mOrthoModeSize);
		ox -= ptr->mViewWidth / 2.0f;
		oy = ptr->mViewHeight / 2.0f - oy;
		ox /= scale;
		oy /= scale;
		lua_pushnumber(L, ox);
		lua_pushnumber(L, oy);
		return 2;
	}

	int ExportedCamera::API_SetCullingMask(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->mCullingFlags=lua_tointeger(L,2);
		return 0;
	}

	int ExportedCamera::API_SetRenderTexture(lua_State*L)
	{
		return 0;
	}

	int ExportedCamera::API_SetClearColor(lua_State*L)
	{
		if (!lua_isnumber(L, -1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3) || !lua_isnumber(L, -4))
		{
			errorC("Camera:SetClearColor arg at index -4,-3,-2,-1 must be a number");
			return 0;
		}
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->SetClearColor(lua_tointeger(L, -4), lua_tointeger(L, -3), lua_tointeger(L, -2), lua_tointeger(L, -1));
		return 0;
	}

	int ExportedCamera::API_SetClipNearFar(lua_State*L)
	{
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->mNearClip = lua_tonumber(L, 2);
		ptr->mFarClip = lua_tonumber(L, 3);
		ptr->OnViewPortSizeChange(ptr->mViewWidth, ptr->mViewHeight);
		return 0;
	}

	int ExportedCamera::API_SetFOV(lua_State*L){
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		ptr->mFieldOfView = lua_tonumber(L, 2);
		ptr->OnViewPortSizeChange(ptr->mViewWidth, ptr->mViewHeight);
		return 0;
	}
	int ExportedCamera::API_GetDirection(lua_State*L) {
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		lua_pushnumber(L, -ptr->mViewMatrix.mData[2]);
		lua_pushnumber(L, -ptr->mViewMatrix.mData[6]);
		lua_pushnumber(L, -ptr->mViewMatrix.mData[10]);
		return 3;
	}
	int ExportedCamera::API_GetRight(lua_State*L) {
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		lua_pushnumber(L, ptr->mViewMatrix.mData[0]);
		lua_pushnumber(L, ptr->mViewMatrix.mData[4]);
		lua_pushnumber(L, ptr->mViewMatrix.mData[8]);
		return 3;
	}
	int ExportedCamera::API_GetUp(lua_State*L) {
		ExportedCamera*ptr = TO_USERDATA(L, ExportedCamera, 1);
		lua_pushnumber(L, ptr->mViewMatrix.mData[1]);
		lua_pushnumber(L, ptr->mViewMatrix.mData[5]);
		lua_pushnumber(L, ptr->mViewMatrix.mData[9]);
		return 3;
	}
}
