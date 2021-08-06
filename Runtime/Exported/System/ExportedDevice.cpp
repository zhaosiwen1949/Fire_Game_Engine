#include "ExportedDevice.h"
#include "Runtime/Device/DeviceInfo.h"
namespace Alice {
	int ExportedDevice::API_Mac(lua_State*L) {
		lua_pushstring(L, DeviceInfo::Mac());
		return 1;
	}
	int ExportedDevice::API_GPUVendor(lua_State*L) {
		lua_pushstring(L, DeviceInfo::GPUVender());
		return 1;
	}
	int ExportedDevice::API_GPURenderer(lua_State*L) {
		lua_pushstring(L, DeviceInfo::GPURenderer());
		return 1;
	}

	int ExportedDevice::API_GLVersion(lua_State*L) {
		lua_pushstring(L, DeviceInfo::GLVersion());
		return 1;
	}

	int ExportedDevice::API_GLSLVersion(lua_State*L) {
		lua_pushstring(L, DeviceInfo::GLSLVersion());
		return 1;
	}

	int ExportedDevice::API_CPUCount(lua_State*L) {
		lua_pushinteger(L, DeviceInfo::GetCPUCoreCount());
		return 1;
	}

	int ExportedDevice::API_IsMobile(lua_State*L) {
		bool ret = false;
#if ALICE_IPHONE
		ret = true;
#elif ALICE_ANDROID
		ret = true;
#elif ALICE_WIN_PLAYER
#elif ALICE_EDITOR
#elif ALICE_OSX_PLAYER
#endif
		lua_pushboolean(L, ret);
		return 1;
	}
    int ExportedDevice::API_KeepScreenOn(lua_State *L){
#if ALICE_IPHONE
        iOSAPIBridge::KeeyScreenOn(lua_toboolean(L, 1));
#elif ALICE_ANDROID
        
#elif ALICE_WIN_PLAYER
#elif ALICE_EDITOR
#elif ALICE_OSX_PLAYER
#endif
        return 0;
    }
	int ExportedDevice::API_SupportAPI(lua_State*L) {
		lua_pushboolean(L, DeviceInfo::SupportOpenGLAPI(lua_tostring(L,1)));
		return 1;
	}
	int ExportedDevice::API_ClientResolution(lua_State*L) {
		lua_pushinteger(L, DeviceInfo::GetClientWidth());
		lua_pushinteger(L, DeviceInfo::GetClientHeight());
		return 2;
	}
	int ExportedDevice::API_DesktopResolution(lua_State*L) {
		lua_pushinteger(L, DeviceInfo::GetScreenWidth());
		lua_pushinteger(L, DeviceInfo::GetScreenHeight());
		return 2;
	}
	int ExportedDevice::API_MemorySize(lua_State*L) {
		lua_pushinteger(L,DeviceInfo::GetMemorySize());
		return 1;
	}
	int ExportedDevice::API_CPUVendor(lua_State*L) {
		lua_pushstring(L, DeviceInfo::CPUVender());
		return 1;
	}
	int ExportedDevice::API_Model(lua_State*L) {
		lua_pushstring(L, DeviceInfo::CPUVender());
		return 1;
	}
	int ExportedDevice::API_CPUType(lua_State*L) {
		lua_pushstring(L, DeviceInfo::CPUType());
		return 1;
	}
	int ExportedDevice::API_SystemName(lua_State*L) {
		lua_pushstring(L, DeviceInfo::SystemName());
		return 1;
	}
	int ExportedDevice::API_DPIX(lua_State*L) {
		lua_pushnumber(L, DeviceInfo::GetDPIX());
		return 1;
	}
	int ExportedDevice::API_DPIY(lua_State*L) {
		lua_pushnumber(L, DeviceInfo::GetDPIY());
		return 1;
	}
	int ExportedDevice::API_DPI(lua_State*L) {
		lua_pushnumber(L, DeviceInfo::GetDPI());
		return 1;
	}
	int ExportedDevice::API_NativeScale(lua_State*L) {
		lua_pushnumber(L, DeviceInfo::GetScale());
		return 1;
	}
    int ExportedDevice::API_DeviceInfo(lua_State*L) {
        lua_pushstring(L, DeviceInfo::GetDeviceInfo());
        return 1;
    }
	int ExportedDevice::Export(lua_State*L){
		luaL_Reg api_c[] =
		{
			EXPORT_API(Mac)
			EXPORT_API(CPUCount)
			EXPORT_API(CPUVendor)
			EXPORT_API(CPUType)
			EXPORT_API(MemorySize)
			EXPORT_API(SystemName)
			EXPORT_API(GPUVendor)
			EXPORT_API(GPURenderer)
			EXPORT_API(GLVersion)
			EXPORT_API(GLSLVersion)
			EXPORT_API(IsMobile)
            EXPORT_API(KeepScreenOn)
			EXPORT_API(SupportAPI)
			EXPORT_API(DesktopResolution)
			EXPORT_API(ClientResolution)
			EXPORT_API(DPIX)
			EXPORT_API(DPIY)
			EXPORT_API(DPI)
			EXPORT_API(Model)
			EXPORT_API(NativeScale)
            EXPORT_API(DeviceInfo)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Device, api_c);
		return 0;
	}
}
