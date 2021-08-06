#import "Utils.h"
#import <CommonCrypto/CommonDigest.h>

@implementation Utils

+(void)CreateMD5:(char *)data dataLen:(int)nLen MD5:(char *)md5
{
    unsigned char result[32];
    CC_MD5(data, nLen, result);
    NSString *md5Str=[NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]];
    memcpy(md5, [md5Str UTF8String], 32);
}
@end
