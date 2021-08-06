#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Device/DeviceInfo.h"

namespace Alice{
	class ExportedDevice{
	public:
		DEFINE_LUA_API(Mac);
		DEFINE_LUA_API(CPUCount);
		DEFINE_LUA_API(CPUVendor);
		DEFINE_LUA_API(CPUType);
		DEFINE_LUA_API(MemorySize);
		DEFINE_LUA_API(GPUVendor);
		DEFINE_LUA_API(GPURenderer);
		DEFINE_LUA_API(GLVersion);
		DEFINE_LUA_API(GLSLVersion);
		DEFINE_LUA_API(SystemName);
		DEFINE_LUA_API(Model);
        DEFINE_LUA_API(DeviceInfo);
		DEFINE_LUA_API(IsMobile);
        DEFINE_LUA_API(KeepScreenOn);
		DEFINE_LUA_API(SupportAPI);
		DEFINE_LUA_API(DesktopResolution);
		DEFINE_LUA_API(ClientResolution);
		DEFINE_LUA_API(DPIX);
		DEFINE_LUA_API(DPIY);
		DEFINE_LUA_API(DPI);
		DEFINE_LUA_API(NativeScale);
		static int Export(lua_State*L);
	};
}
