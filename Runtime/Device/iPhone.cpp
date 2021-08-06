#include "iPhone.h"
#include "Runtime/Render/AliceGL.h"
#ifdef ALICE_IPHONE
namespace Alice{
    static int sFeatures = 0;
    static float sDPIX = 0.0f, sDPIY = 0.0f, sDPI = 0.0f, sScreenScale = 1.0f;
    static int sScreenWidth = 0;
    static int sScreenHeight = 0;
    static int sClientWidth = 0;
    static int sClientHeight = 0;
    static int sCPUCoreCount = 0;
    static int sMemorySize = 0;
    static char sCPUVendor[256] = { 0 };
    static char sCPUType[256] = { 0 };
    static char sSystemName[256] = { 0 };
    static char sDeviceInfo[256] = { 0 };
    static char sGPUVendor[256] = { 0 };
    static char sGPURenderer[256] = { 0 };
    static char sGLVersion[256] = { 0 };
    static char sGLSLVersion[256] = { 0 };
    static int sMaxTextureSize = 0;
    void DeviceInfo::SetSize(int width, int height, float scale, float dpix, float dpiy, float dpi) {
        sScreenWidth = width;
        sScreenHeight = height;
        sClientWidth = width;
        sClientHeight = height;
        sScreenScale = scale;
        sDPIX = dpix;
        sDPIY = dpiy;
        sDPI = dpi;
        //Info("Screen Info : %d x %d scale(%f) dpi(%f x[%f] y[%f])",width,height,scale,dpi,dpix,dpiy);
    }
    void DeviceInfo::SetSystemInfo(int core_count, const char * device_name, const char *cpu_type, const char * system_name, int memorysize) {
        sCPUCoreCount = core_count;
        strcpy(sDeviceInfo, device_name);
        strcpy(sCPUType, cpu_type);
        strcpy(sSystemName, system_name);
        sMemorySize = memorysize;
        //Info("CPU Core Count : %d", sCPUCoreCount);
        //Info("CPU Vendor : %s", sCPUVendor);
        //Info("CPU Type : %s", sCPUType);
        //Info("System Name : %s", sSystemName);
        //Info("Memory Size : %d", sMemorySize);
    }
    int DeviceInfo::GetScreenWidth() {
        return sScreenWidth;
    }
    int DeviceInfo::GetScreenHeight() {
        return sScreenHeight;
    }
    int DeviceInfo::GetClientWidth() {
        return sClientWidth;
    }
    int DeviceInfo::GetClientHeight() {
        return sClientHeight;
    }
    int DeviceInfo::GetCPUCoreCount() {
        return sCPUCoreCount;
    }
    int DeviceInfo::GetMemorySize() {
        return sMemorySize;
    }
    float DeviceInfo::GetScale() {
        return sScreenScale;
    }
    const char * DeviceInfo::CPUVender() {
        return sCPUVendor;
    }
    const char * DeviceInfo::CPUType() {
        return sCPUType;
    }
    const char* DeviceInfo::SystemName() {
        return sSystemName;
    }
    const char*DeviceInfo::GPUVender() {
        if (sGPUVendor[0] == 0) {
            strcpy(sGPUVendor, (const char*)glGetString(GL_VENDOR));
        }
        return sGPUVendor;
    }
    const char*DeviceInfo::GPURenderer() {
        if (sGPURenderer[0] == 0) {
            strcpy(sGPURenderer, (const char*)glGetString(GL_RENDERER));
        }
        return sGPURenderer;
    }
    const char*DeviceInfo::GLVersion() {
        if (sGLVersion[0] == 0) {
            strcpy(sGLVersion, (const char*)glGetString(GL_VERSION));
        }
        return sGLVersion;
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
        if (strcasecmp(feature_name, "NEON") == 0) {
            return true;
        }
        return false;
    }
    bool DeviceInfo::SupportOpenGLAPI(const char *api_name) {
        return true;
    }
    float DeviceInfo::GetDPIX() {
        return sDPIX;
    }
    float DeviceInfo::GetDPIY() {
        return sDPIY;
    }
    float DeviceInfo::GetDPI() {
        return sDPI;
    }
    const char * DeviceInfo::GetDeviceInfo(){
        return sDeviceInfo;
    }
}
#endif
