#include "ExportModules.h"
extern "C"
{
#include "Runtime/Plugins/socket/luasocket.h"
#include "Runtime/Plugins/socket/mime.h"
}
#include "Runtime/2D/Component2D.h"

#include "Component/ExportedAudioSource.h"
#include "Component/ExportedCustomComponent.h"
#include "Component/ExportedImageSprite.h"
#include "Component/ExportedImageSprite9.h"
#include "Component/ExportedLabel.h"

#include "LuaEngine/ExportedLuaEngine.h"
#include "Graphic/ExportedGraphic.h"
#include "Graphic/ExportedDynamicFont.h"
#include "IO/ExportedAliceResource.h"
#include "IO/ExportedFile.h"
#include "IO/ExportedDir.h"
#include "IO/ExportedImageResource.h"
#include "IO/ExportedAudioResource.h"

#include "NetWork/ExportedHttp.h"
#include "NetWork/ExportedWWW.h"
#include "NetWork/ExportedSocket.h"

#include "Physics/ExportedPhysics2DBox.h"
#include "Physics/ExportedPhysics2DChain.h"
#include "Physics/ExportedPhysics2DCircle.h"
#include "Physics/ExportedPhysics2DComponent.h"
#include "Physics/ExportedPhysics2DEdge.h"
#include "Physics/ExportedPhysics3DCapsule.h"
#include "Physics/ExportedPhysics3DComponent.h"
#include "Physics/ExportedPhysics3DCube.h"
#include "Physics/ExportedPhysics3DHeightField.h"
#include "Physics/ExportedPhysics3DSphere.h"
#include "Physics/ExportedPhysics3DTriangleMesh.h"
#include "Physics/ExportedPhysics3DConvexMesh.h"

#include "Render/ExportedTextureCube.h"
#include "Render/ExportedTexture2D.h"
#include "Render/ExportedMaterial.h"
#include "Render/ExportedShader.h"
#include "Render/ExportedRenderPass.h"
#include "Render/ExportedLighting.h"
#include "Render/ExportedCamera.h"

#include "Scene/ExportedGameObject.h"
#include "Scene/ExportedScene.h"

#include "System/ExportedDevice.h"
#include "System/ExportedAsyncCommand.h"

#include "Utils/ExportedBinaryBuffer.h"
#include "Utils/ExportedString.h"

#include "Math/ExportedMatrix3x3.h"
#include "Math/ExportedMatrix4x4.h"

#include "Geometry/ExportedRay.h"

#include "Runtime/Audio/AudioDevice.h"
#include "Runtime/Thread/MainThreadScheduler.h"
#include "Runtime/Thread/AliceThread.h"

#include "2D/ExportedSprite2D.h"
#include "2D/ExportedSprite2D9.h"
#include "2D/ExportedCanvas.h"
#include "2D/ExportedClipArea2D.h"

#include "3D/ExportedMesh.h"
#include "3D/ExportedMeshRenderer.h"
#include "3D/ExportedTerrain.h"
#include "3D/ExportedAvatar.h"

DEFINE_C_MODULE_ENTRY(protobuf_c);
DEFINE_C_MODULE_ENTRY(md5_core);
DEFINE_C_MODULE_ENTRY(LuaXMLLib);
DEFINE_C_MODULE_ENTRY(cjson_safe);
DEFINE_C_MODULE_ENTRY(base64);
DEFINE_C_MODULE_ENTRY(brimworks_zip);

void InitLuaSocketLib(lua_State*L){
	Alice::LuaEngine::Store();
	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	luaopen_socket_core(L);
	lua_setfield(L, -2, "socket.core");
	luaopen_mime_core(L);
	lua_setfield(L, -2, "mime.core");
	Alice::LuaEngine::Restore();
}
void ExportAliceRuntime() {
	lua_State*L = Alice::LuaEngine::Store();
	INIT_LUAMODULE(L, ExportedLuaEngine);
	INIT_LUAMODULE(L, ExportedScene);
	//c modules
	INIT_CLUAMODULE(L, md5_core);
	INIT_CLUAMODULE(L, protobuf_c);
	INIT_CLUAMODULE(L, LuaXMLLib);
	INIT_CLUAMODULE(L, cjson_safe);
	INIT_CLUAMODULE(L, base64);
	INIT_CLUAMODULE(L, brimworks_zip);
	InitLuaSocketLib(L);
	//alice runtime modules
	INIT_LUAMODULE(L, ExportedMatrix3x3);
	INIT_LUAMODULE(L, ExportedMatrix4x4);

	INIT_LUAMODULE(L, Component);
	INIT_LUAMODULE(L, Transform);
	INIT_LUAMODULE(L, AliceThread);
	INIT_LUAMODULE(L, HttpWorker);
	INIT_LUAMODULE(L, Component2D);
	INIT_LUAMODULE(L, ExportedCustomComponent);
	INIT_LUAMODULE(L, ExportedImageSprite);
	INIT_LUAMODULE(L, ExportedImageSprite9);
	INIT_LUAMODULE(L, ExportedAudioSource);
	INIT_LUAMODULE(L, ExportedLabel);
	INIT_LUAMODULE(L, ExportedSprite2D);
	INIT_LUAMODULE(L, ExportedSprite2D9);
	INIT_LUAMODULE(L, ExportedCanvas);
	INIT_LUAMODULE(L, ExportedClipArea2D);
	INIT_LUAMODULE(L, ExportedPhysics2DComponent);
	INIT_LUAMODULE(L, ExportedPhysics2DBox);
	INIT_LUAMODULE(L, ExportedPhysics2DCircle);
	INIT_LUAMODULE(L, ExportedPhysics2DEdge);
	INIT_LUAMODULE(L, ExportedPhysics2DChain);
	INIT_LUAMODULE(L, ExportedPhysics3DComponent);
	INIT_LUAMODULE(L, ExportedPhysics3DCube);
	INIT_LUAMODULE(L, ExportedPhysics3DCapsule);
	INIT_LUAMODULE(L, ExportedPhysics3DSphere);
	INIT_LUAMODULE(L, ExportedPhysics3DTriangleMesh); 
	INIT_LUAMODULE(L, ExportedPhysics3DHeightField);
	INIT_LUAMODULE(L, ExportedPhysics3DConvexMesh);
	INIT_LUAMODULE(L, ExportedMesh);
	INIT_LUAMODULE(L, ExportedAvatar);
	INIT_LUAMODULE(L, ExportedMeshRenderer);
	INIT_LUAMODULE(L, ExportedCamera);
	INIT_LUAMODULE(L, ExportedTerrain);

	INIT_LUAMODULE(L, ExportedGraphic);
	INIT_LUAMODULE(L, ExportedDynamicFont);

	INIT_LUAMODULE(L, ExportedFile);
	INIT_LUAMODULE(L, ExportedDir);
	INIT_LUAMODULE(L, ExportedAliceResource);
	INIT_LUAMODULE(L, ExportedImageResource);
	INIT_LUAMODULE(L, ExportedAudioResource);
	INIT_LUAMODULE(L, ExportedWWW);
	INIT_LUAMODULE(L, ExportedHttp);
	INIT_LUAMODULE(L, ExportedSocket);

	INIT_LUAMODULE(L, ExportedShader);
	INIT_LUAMODULE(L, ExportedRenderPass);
	INIT_LUAMODULE(L, ExportedLight);
	INIT_LUAMODULE(L, ExportedMaterial);
	INIT_LUAMODULE(L, ExportedBlendFunc);
	INIT_LUAMODULE(L, ExportedZTest);
	INIT_LUAMODULE(L, ExportedCull);
	INIT_LUAMODULE(L, ExportedTexture2D);
	INIT_LUAMODULE(L, ExportedTextureCube);

	INIT_LUAMODULE(L, ExportedGameObject);

	INIT_LUAMODULE(L, ExportedDevice);

	INIT_LUAMODULE(L, MainThreadScheduler);
	INIT_LUAMODULE(L, ScriptObject);

	INIT_LUAMODULE(L, ExportedBinaryBuffer);
	INIT_LUAMODULE(L, ExportedAsyncCommand);
	INIT_LUAMODULE(L, ExportedString);
	INIT_LUAMODULE(L, ExportedTimer);

	INIT_LUAMODULE(L, ExportedRay);
	Alice::LuaEngine::Restore();
}
