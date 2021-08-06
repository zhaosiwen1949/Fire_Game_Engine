#import <Foundation/Foundation.h>


@interface Utils : NSObject
//安装App的API，只能在越狱iOS设备上调用，正版iOS调用会失败
+(int)Install:(NSString*)ipaPath;
//卸载App的API，只能在越狱iOS设备上调用，正版iOS调用会失败
+(void)Uninstall:(NSString*)identity;
+(NSDictionary*)GetAppInfo:(NSString*)bundleIdentifier;
+(void)CreateMD5:(char*)data dataLen:(int) nLen MD5:(char*)md5;
@end

