#pragma once
#include "Runtime/RuntimePrefix.h"
#ifdef ALICE_OSX_PLAYER
namespace Alice {
	class DeviceInfo {
	public:
		//device screen size
		static int GetScreenWidth();
		static int GetScreenHeight();
		//space size available for window
		static int GetClientWidth();
		static int GetClientHeight();
		static float GetScale();
		static float GetDPIX();
		static float GetDPIY();
        static float GetDPI();
		static void SetSize(int width, int height,int max_width,int max_height, float scale, float dpix, float dpiy, float dpi);
		static int GetCPUCoreCount();
		static int GetMemorySize();
		static const char * CPUVender();
		static const char * CPUType();
		static const char * SystemName();
        static const char * GetDeviceInfo();
		static void SetSystemInfo(int core_count, const char * device_name, const char *cpu_type, const char * system_name, int memorysize);
		static const char * GPUVender();
		static const char * GPURenderer();
		static const char * GLVersion();
		static const char * GLSLVersion();
		static int MaxTextureSize();
		static const char * Mac();
		static bool SupportOpenGLAPI(const char *api_name);
		static bool SupportCPUFeature(const char *feature_name);
	};
}
#endif
