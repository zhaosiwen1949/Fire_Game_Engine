#pragma once
#include "Runtime/RuntimePrefix.h"
#ifdef ALICE_PLATFORM_WIN
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
		static int GetCPUCoreCount();
		static int GetMemorySize();
		static const char * CPUVender();
		static const char * CPUType();
		static const char * SystemName();
		static const char * GPUVender();
		static const char * GPURenderer();
		static const char * GLVersion();
		static const int GLMajorVersion();
		static const int GLMinorVersion();
		static const char * GLSLVersion();
		static int MaxTextureSize();
		static const char * Mac();
		static bool SupportOpenGLAPI(const char *api_name);
		static bool SupportCPUFeature(const char *feature_name);
	};
}
#endif