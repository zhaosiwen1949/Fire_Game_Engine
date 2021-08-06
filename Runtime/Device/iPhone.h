#pragma once
#include "Runtime/RuntimePrefix.h"
#ifdef ALICE_IPHONE
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
        static const char * GetDeviceInfo();
		static void SetSize(int width, int height, float scale, float dpix, float dpiy, float dpi);
		static int GetCPUCoreCount();
		static int GetMemorySize();
		static const char * CPUVender();
		static const char * CPUType();
		static const char * SystemName();
		static void SetSystemInfo(int core_count, const char * info_str, const char *cpu_type, const char * system_name, int memorysize);
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
