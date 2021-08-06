#include "Win.h"
#include "Runtime/Render/AliceGL.h"
#ifdef ALICE_PLATFORM_WIN
#include <ShellScalingApi.h>
#include <intrin.h>
#include <VersionHelpers.h>
#define CPUID_FEATURES_SSE  (1 << 25)
#define CPUID_FEATURES_SSE2 (1 << 26)
#define WINVERSION_2012 4026541440 //Microsoft Windows Server 2012 R2 的BuildNumber号
#define WINVERSION_10 4026546233 //Microsoft Windows 10 的BuildNumber号
namespace Alice{
	static int sFeatures = 0;
	static float sDPIX = 0.0f, sDPIY = 0.0f, sScreenScale = 1.0f;
	static int sScreenWidth = 0;
	static int sScreenHeight = 0;
	static int sClientWidth = 0;
	static int sClientHeight = 0;
	static int sScreenSizeWidth = 0;
	static int sScreenSizeHeight = 0;
	static int sCPUCoreCount = 0;
	static int sMemorySize = 0;
	static char sCPUVendor[256] = { 0 };
	static char sCPUType[256] = { 0 };
	static char sSystemName[256] = { 0 };
	static char sGPUVendor[256] = { 0 };
	static char sGPURenderer[256] = { 0 };
	static char sGLVersion[256] = { 0 };
	static int sMajorVersion = -1,sMinorVersion = -1;
	static char sGLSLVersion[256] = { 0 };
	static int sMaxTextureSize = 0;
	void InitDisplayInfo() {
		HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
		if (monitor) {
			MONITORINFO info;
			info.cbSize = sizeof(info);
			GetMonitorInfo(monitor, &info);
			sClientWidth = info.rcWork.right - info.rcWork.left;
			sClientHeight = info.rcWork.bottom - info.rcWork.top;
			sScreenWidth = info.rcMonitor.right - info.rcMonitor.left;
			sScreenHeight = info.rcMonitor.bottom - info.rcMonitor.top;
		}
	}
	void InitSystemInfo() {
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		sCPUCoreCount = si.dwNumberOfProcessors;
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		sMemorySize=(statex.ullTotalPhys / 1024) / 1024;
		if (IsWindows10OrGreater()){
			strcpy(sSystemName, "Windows 10");
		}
		else if (IsWindows8Point1OrGreater()) {
			strcpy(sSystemName, "Windows 8.1");
		}
		else if (IsWindows8OrGreater()) {
			strcpy(sSystemName, "Windows 8");
		}
		else if (IsWindows7SP1OrGreater()) {
			strcpy(sSystemName, "Windows 7 SP1");
		}
		else if (IsWindows7OrGreater()) {
			strcpy(sSystemName, "Windows 7");
		}
		else if (IsWindowsVistaSP2OrGreater()) {
			strcpy(sSystemName, "Windows Vista SP2");
		}
		else if (IsWindowsVistaSP1OrGreater()) {
			strcpy(sSystemName, "Windows Vista SP1");
		}
		else if (IsWindowsVistaOrGreater()) {
			strcpy(sSystemName, "Windows Vista");
		}
		else {
			strcpy(sSystemName, "Unkown");
		}
	}
	void InitCPUInfo() {
		int CPUInfo[4] = { -1 };
		unsigned   nExIds, i = 0;
		__cpuid(CPUInfo, 0);
		memcpy(sCPUVendor, &CPUInfo[1], sizeof(CPUInfo[0]));
		memcpy(sCPUVendor + 4, &CPUInfo[3], sizeof(CPUInfo[0]));
		memcpy(sCPUVendor + 8, &CPUInfo[2], sizeof(CPUInfo[0]));
		__cpuid(CPUInfo, 0x80000000);
		nExIds = CPUInfo[0];
		for (i = 0x80000000; i <= nExIds; ++i){
			__cpuid(CPUInfo, i);
			if (i == 0x80000002)
				memcpy(sCPUType, CPUInfo, sizeof(CPUInfo));
			else if (i == 0x80000003)
				memcpy(sCPUType + 16, CPUInfo, sizeof(CPUInfo));
			else if (i == 0x80000004)
				memcpy(sCPUType + 32, CPUInfo, sizeof(CPUInfo));
		}
	}
	void InitDeviceDPIInfo() {
		if (sCPUCoreCount==0){
			InitSystemInfo();
		}
		if (IsWindows8OrGreater()) {
			HMONITOR hMonitor;
			POINT    point;
			HRESULT  hr = E_FAIL;
			point.x = 1;
			point.y = 1;
			hMonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
			UINT DPIX, DPIY;
			typedef HRESULT(__stdcall*NTPROC)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);
			HINSTANCE hinst = LoadLibrary(L"Shcore.dll");
			NTPROC proc = (NTPROC)GetProcAddress(hinst, "GetDpiForMonitor");
			hr = proc(hMonitor, MDT_EFFECTIVE_DPI, &DPIX, &DPIY);
			sDPIX = DPIX;
			sDPIY = DPIY;
		}
		else {
			HDC hdc = GetDC(NULL);
			if (hdc) {
				sDPIX = GetDeviceCaps(hdc, LOGPIXELSX);
				sDPIY = GetDeviceCaps(hdc, LOGPIXELSY);
				ReleaseDC(NULL, hdc);
			}
		}
		sScreenScale = float(sDPIX) / 96.0f;
	}
	int DeviceInfo::GetScreenWidth() {
		if (sScreenWidth==0){
			InitDisplayInfo();
		}
		return sScreenWidth;
	}
	int DeviceInfo::GetScreenHeight() {
		if (sScreenHeight == 0) {
			InitDisplayInfo();
		}
		return sScreenHeight;
	}
	int DeviceInfo::GetClientWidth() {
		if (sClientWidth == 0) {
			InitDisplayInfo();
		}
		return sClientWidth;
	}
	int DeviceInfo::GetClientHeight() {
		if (sClientHeight == 0) {
			InitDisplayInfo();
		}
		return sClientHeight;
	}
	int DeviceInfo::GetCPUCoreCount() {
		if (sCPUCoreCount == 0) {
			InitSystemInfo();
		}
		return sCPUCoreCount;
	}
	int DeviceInfo::GetMemorySize() {
		if (sMemorySize == 0) {
			InitSystemInfo();
		}
		return sMemorySize;
	}
	float DeviceInfo::GetScale() {
		return sScreenScale;
	}
	const char * DeviceInfo::CPUVender() {
		if (sCPUVendor[0]==0){
			InitCPUInfo();
		}
		return sCPUVendor;
	}
	const char * DeviceInfo::CPUType() {
		if (sCPUType[0] == 0) {
			InitCPUInfo();
		}
		return sCPUType;
	}
	const char* DeviceInfo::SystemName() {
		if (sSystemName[0]==0){
			InitSystemInfo();
		}
		return sSystemName;
	}
	const char*DeviceInfo::GPUVender() {
		if (sGPUVendor[0] == 0) {
			strcpy (sGPUVendor,(const char*)glGetString(GL_VENDOR));
		}
		return sGPUVendor;
	}
	const char*DeviceInfo::GPURenderer() {
		if (sGPURenderer[0] == 0) {
			strcpy(sGPURenderer, (const char*)glGetString(GL_RENDERER));
		}
		return sGPURenderer;
	}
	const char*DeviceInfo::GetDeviceInfo() {
		return "WindowsPlatform";
	}
	const char*DeviceInfo::GLVersion() {
		if (sGLVersion[0] == 0) {
			const char * glVersion = (const char*)glGetString(GL_VERSION);
			if (glVersion!=nullptr){
				strcpy(sGLVersion, glVersion);
			}
			else {
				strcpy(sGLVersion, "unkown");
			}
			
		}
		return sGLVersion;
	}
	const int DeviceInfo::GLMajorVersion() {
		if (sMajorVersion=-1){
			glGetIntegerv(GL_MAJOR_VERSION, &sMajorVersion);
		}
		return sMajorVersion;

	}
	const int DeviceInfo::GLMinorVersion() {
		if (sMinorVersion = -1) {
			glGetIntegerv(GL_MINOR_VERSION, &sMinorVersion);
		}
		return sMinorVersion;
	}
	const char*DeviceInfo::GLSLVersion() {
		if (sGLSLVersion[0] == 0) {
			strcpy(sGLSLVersion, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		}
		return sGLSLVersion;
	}
	int DeviceInfo::MaxTextureSize() {
		if (sMaxTextureSize == 0) {
			GLint temp;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &temp);
			sMaxTextureSize = temp;
		}
		return sMaxTextureSize;
	}
	const char * DeviceInfo::Mac() {
		return "Unkown";
	}
	bool DeviceInfo::SupportCPUFeature(const char *feature_name) {
		if (_stricmp(feature_name, "SSE") == 0){
			return (sFeatures & CPUID_FEATURES_SSE) != 0;
		}else if (_stricmp(feature_name, "SSE2") == 0) {
			return (sFeatures & CPUID_FEATURES_SSE2) != 0;
		}
		return false;
	}
	bool DeviceInfo::SupportOpenGLAPI(const char *api_name) {
		if (NULL == wglGetProcAddress(api_name)) {
			return false;
		}
		return true;
	}
	float DeviceInfo::GetDPIX() {
		if (sDPIX==0.0f){
			InitDeviceDPIInfo();
		}
		return sDPIX;
	}
	float DeviceInfo::GetDPIY() {
		if (sDPIY == 0.0f) {
			InitDeviceDPIInfo();
		}
		return sDPIY;
	}
	float DeviceInfo::GetDPI() {
		if (sDPIX == 0.0f) {
			InitDeviceDPIInfo();
		}
		return sDPIX;
	}
}
#endif