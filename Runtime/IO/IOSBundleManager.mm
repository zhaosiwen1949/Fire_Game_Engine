#import "IOSBundleManager.h"
#import <Foundation/Foundation.h>
#ifdef ALICE_OSX_PLAYER
#import <AppKit/AppKit.h>
#else
#import <UIKit/UIKit.h>
#endif
namespace  Alice
{
    bool IOSBundleManager::LoadInternalData(const char *path, Alice::Data &data)
    {
        NSString *nsPath = [[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:path] ofType:nil]retain];
        NSData *_data=[[NSData dataWithContentsOfFile:nsPath]retain];
		int file_size=[_data length];
        if (file_size>0) {
            data.SetBufferSize(file_size+1);
            memcpy(data.mData, [_data bytes], file_size);
			data.mData[file_size]='\0';
            data.mDataLen=file_size;
            [nsPath release];
            [_data release];
        }
        else
        {
            return false;
        }
        return true;
    }
    
    const char* IOSBundleManager::GetOuterDataPath()
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *docDir = [paths objectAtIndex:0];
        return [docDir UTF8String];
    }
    
    const char* IOSBundleManager::GetDesktopPath()
    {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, YES );
        NSString* theDesktopPath = [paths objectAtIndex:0];
        return [theDesktopPath UTF8String];
    }
    
    void IOSBundleManager::OpenURL(const char *url){
#ifdef ALICE_OSX_PLAYER
        [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
#else
        NSString*urlNS=[NSString stringWithUTF8String:url];
        [[UIApplication sharedApplication]openURL:[NSURL URLWithString:urlNS]];
#endif
    }
    void IOSBundleManager::GetItemListInDirectory(lua_State *L, const char *path){
        NSString* folderPath = [NSString stringWithUTF8String:path];
        NSFileManager *fileManager=[NSFileManager defaultManager];
        NSArray*items=[fileManager contentsOfDirectoryAtPath: folderPath error:nil];
        int nIndex=1;
        for (NSString *fileName in items)
        {
            BOOL isDir=NO;
            NSString*fullPath=[folderPath stringByAppendingPathComponent:fileName];
            [fileManager fileExistsAtPath:fullPath isDirectory:&isDir];
            lua_pushinteger(L, nIndex++);
            lua_newtable(L);
            lua_pushstring(L, [fileName UTF8String]);
            lua_setfield(L, -2, "name");
            if(isDir){
                lua_pushboolean(L, true);
                lua_setfield(L, -2, "is_dir");
                lua_settable(L, -3);
            }else{
                lua_pushboolean(L, false);
                lua_setfield(L, -2, "is_dir");
                lua_settable(L, -3);
            }
        }
    }
    bool IOSBundleManager::DeleteFileWithPath(const char * path){
        NSString* filePath = [NSString stringWithUTF8String:path];
        NSFileManager *fileManager=[NSFileManager defaultManager];
        BOOL isDir=NO;
        if([fileManager fileExistsAtPath:filePath isDirectory:&isDir]){
            [fileManager removeItemAtPath:filePath error:nil];
            return true;
        }
        return false;
    }
    bool IOSBundleManager::DeleteDir(const char * path){
        NSString* filePath = [NSString stringWithUTF8String:path];
        NSFileManager *fileManager=[NSFileManager defaultManager];
        BOOL isDir=NO;
        if([fileManager fileExistsAtPath:filePath isDirectory:&isDir]){
            [fileManager removeItemAtPath:filePath error:nil];
            return true;
        }
        return false;
    }
}
