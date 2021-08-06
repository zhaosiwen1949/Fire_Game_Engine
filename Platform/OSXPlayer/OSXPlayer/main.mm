//
//  main.m
//  XEdu
//
//  Created by Heck on 2017/7/17.
//  Copyright © 2017年 battlefire. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MainWindow.h"
#import "AppDelegate.h"

int main(int argc, const char * argv[]) {
    NSApplication*application=[NSApplication sharedApplication];
    AppDelegate*appDelegate=[[AppDelegate alloc]init];
    [application setDelegate:appDelegate];
    [NSApp run];
    return 0;
}
