#import "Utils.h"
#import <dlfcn.h>
#import <CommonCrypto/CommonDigest.h>

typedef void (*MobileInstallationCallback)(CFDictionaryRef information);
typedef int (*MobileInstallationInstall)(NSString *path, NSDictionary *dict, void *na, NSString *backpath);
typedef CFDictionaryRef (*MobileInstallationLookup)(CFDictionaryRef properties);
typedef int (*MobileInstallationUpgrade)(CFStringRef path, CFDictionaryRef parameters, MobileInstallationCallback callback, void *unknown);
typedef int (*MobileInstallationUninstall)(CFStringRef bundleIdentifier, CFDictionaryRef parameters, MobileInstallationCallback callback, void *unknown);
NSString *MobileInstallationPath=@"/System/Library/PrivateFrameworks/MobileInstallation.framework/MobileInstallation";
@implementation Utils

+(void)CreateMD5:(char *)data dataLen:(int)nLen MD5:(char *)md5
{
    unsigned char result[32];
    CC_MD5(data, nLen, result);
    NSString *md5Str=[NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]];
    memcpy(md5, [md5Str UTF8String], 32);
}
+(NSDictionary*)GetAppInfo:(NSString *)bundleIdentifier
{
    NSDictionary*appInfo=nil;
    static NSString *const cacheFileName = @"com.apple.mobile.installation.plist";
    NSString *relativeCachePath = [[@"Library" stringByAppendingPathComponent: @"Caches"] stringByAppendingPathComponent: cacheFileName];
    NSDictionary *cacheDict = nil;
    NSString *path = nil;
    BOOL isDir=NO;
    path = [[NSHomeDirectory() stringByAppendingPathComponent: @"../.."] stringByAppendingPathComponent: relativeCachePath];
    if ([[NSFileManager defaultManager] fileExistsAtPath: path isDirectory: &isDir] && !isDir)
    {
        cacheDict = [NSDictionary dictionaryWithContentsOfFile: path];
    }
    NSDictionary *user = [cacheDict objectForKey: @"User"];
    if ([user objectForKey:bundleIdentifier]) {
        appInfo=[user objectForKey:bundleIdentifier];
    }
    return appInfo;
}

+(void)Uninstall:(NSString*)identity
{
    void *lib=dlopen([MobileInstallationPath UTF8String], RTLD_LAZY);
    if (lib)
    {
        NSLog(@"MobileInstallation lib linked sucessfully");
        MobileInstallationUninstall   pUninstall=(MobileInstallationUninstall)dlsym(lib, "MobileInstallationUninstall");
        if (pUninstall)
        {
            NSLog(@"uninstall %@",identity);
            int nRet=-1;
            nRet=pUninstall((__bridge CFStringRef)identity,nil,nil,nil);
            if (nRet==0) {
                NSLog(@"uninstall sucess");
            }
            else
            {
                NSLog(@"uninstall fail");
            }
        }
        else
        {
            NSLog(@"Install function detect fail!");
        }
    }
    else
    {
        NSLog(@"cannot link to dynamic lib MobileInstallation");
    }
}

+(int)Install:(NSString*)ipaPath
{
    int nRet=-1;
    void *lib=dlopen([MobileInstallationPath UTF8String], RTLD_LAZY);
    if (lib)
    {
        NSLog(@"MobileInstallation lib linked sucessfully");
        MobileInstallationInstall   pInstall=(MobileInstallationInstall)dlsym(lib, "MobileInstallationInstall");
        if (pInstall)
        {
            NSLog(@"Install function detected and i am going to install %@",ipaPath);
            nRet=pInstall(ipaPath,nil,nil,nil);
            if (nRet==0)
            {
                NSLog(@"Install ipa sucess!");
            }
            else
            {
                NSLog(@"Install ipa fail!");
            }
        }
        else
        {
            NSLog(@"Install function detect fail!");
        }
    }
    else
    {
        NSLog(@"cannot link to dynamic lib MobileInstallation");
    }
    return nRet;
}
@end
