#include "Mac.h"
#ifdef ALICE_OSX_PLAYER
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Debugger/log.h"
#include <stdlib.h>
#include <string.h>
#include <AvailabilityMacros.h>
#ifdef MAC_OS_X_VERSION_10_3
#include <OpenGL/OpenGL.h>
#include <dlfcn.h>
void* NSGLGetProcAddress (const GLubyte *name){
    static void* image = NULL;
    void* addr;
    if (NULL == image)
    {
        image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
    }
    if( !image ) return NULL;
    addr = dlsym(image, (const char*)name);
    if( addr ) return addr;
    return NULL;
}
#else
#include <mach-o/dyld.h>
void* NSGLGetProcAddress (const GLubyte *name)
{
    static const struct mach_header* image = NULL;
    NSSymbol symbol;
    char* symbolName;
    if (NULL == image)
    {
        image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
    }
    /* prepend a '_' for the Unix C symbol mangling convention */
    symbolName = malloc(strlen((const char*)name) + 2);
    strcpy(symbolName+1, (const char*)name);
    symbolName[0] = '_';
    symbol = NULL;
    /* if (NSIsSymbolNameDefined(symbolName))
     symbol = NSLookupAndBindSymbol(symbolName); */
    symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
    free(symbolName);
    if( symbol ) return NSAddressOfSymbol(symbol);
    return NULL;
}
#endif /* MAC_OS_X_VERSION_10_3 */
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
	static char sGPUVendor[256] = { 0 };
	static char sGPURenderer[256] = { 0 };
	static char sGLVersion[256] = { 0 };
	static char sGLSLVersion[256] = { 0 };
	static int sMaxTextureSize = 0;
    void DeviceInfo::SetSize(int width, int height, int max_width,int max_height, float scale, float dpix, float dpiy, float dpi) {
        sScreenWidth = max_width;
        sScreenHeight = max_height;
        sClientWidth = width;
        sClientHeight = height;
        sScreenScale = scale;
        sDPIX = dpix;
        sDPIY = dpiy;
        sDPI = dpi;
        //Info("Screen Info : %d(%d) x %d(%d) scale(%f) dpi(%f x[%f] y[%f])",width,sScreenWidth,height,sScreenHeight,scale,dpi,dpix,dpiy);
    }
    void DeviceInfo::SetSystemInfo(int core_count, const char * device_name, const char *cpu_type, const char * system_name, int memorysize) {
        sCPUCoreCount = core_count;
        strcpy(sCPUVendor, device_name);
        strcpy(sCPUType, cpu_type);
        strcpy(sSystemName, system_name);
        sMemorySize = memorysize;
        /*Info("CPU Core Count : %d", sCPUCoreCount);
        Info("CPU Vendor : %s", sCPUVendor);
        Info("CPU Type : %s", sCPUType);
        Info("System Name : %s", sSystemName);
        Info("Memory Size : %d", sMemorySize);*/
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
    const char* DeviceInfo::GetDeviceInfo(){
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
        return false;
    }
    bool DeviceInfo::SupportOpenGLAPI(const char *api_name) {
        if (NULL == NSGLGetProcAddress((GLubyte*)api_name)) {
            return false;
        }
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
}
#endif
