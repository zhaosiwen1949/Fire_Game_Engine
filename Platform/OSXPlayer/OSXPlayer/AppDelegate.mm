//
//  AppDelegate.m
//  东汉书院
//
//  Created by heck on 2018/12/11.
//  Copyright © 2018 battlefire. All rights reserved.
//

#import "AppDelegate.h"
extern "C" void InitMainWindow();
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    InitMainWindow();
}
- (void)applicationWillTerminate:(NSNotification *)aNotification {
}
@end
