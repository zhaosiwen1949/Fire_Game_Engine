//
//  MainWindow.m
//  东汉书院
//
//  Created by heck on 2018/12/11.
//  Copyright © 2018 battlefire. All rights reserved.
//

#import "MainWindow.h"
#import <Foundation/Foundation.h>
#import <IOKit/IOKitLib.h>
#import "AliceView.h"
#include"Runtime/RuntimePrefix.h"
#include"Runtime/Render/AliceGL.h"

static MainWindow*sWindow=nullptr;
extern "C" void OnViewSizeChange(int x, int y, int width, int height);
static bool GetScreenResolution(float&width,float&height){
    NSArray *screenArray = [NSScreen screens];
    if([screenArray count]<=0){
        return false;
    }
    NSScreen *screen = [screenArray objectAtIndex: 0];
    NSRect screenRect = [screen visibleFrame];
    NSRect desktopRect = [screen frame];
    width=screenRect.size.width;
    height=screenRect.size.height;
    return true;
}
static NSRect AdjustWindow(NSRect screenRect){
    float margin_horizontal=floorf((screenRect.size.width-1280.0f)/2.0f);
    float margin_vertical=floorf((screenRect.size.height-720.0f)/2.0f);
    if(margin_vertical<0.0f){
        margin_vertical=0.0f;
    }
    NSRect windowRect = NSMakeRect(margin_horizontal, margin_vertical, 1280.0f, 720.0f);
    return windowRect;
}
extern "C" void InitMainWindow(){
    sWindow=[MainWindow Instance];
    NSRect windowRect=AdjustWindow([[NSScreen mainScreen] visibleFrame]);
    NSUInteger uiStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
    [sWindow initWithContentRect:windowRect styleMask:uiStyle backing:NSBackingStoreBuffered defer:NO];
    AliceView*view=[AliceView Instance];
    [view InitRenderView:[[sWindow contentView] bounds]];
    [[sWindow contentView] addSubview:view];
}
@implementation MainWindow
+(MainWindow*)Instance{
    if(sWindow!=nullptr){
        return sWindow;
    }
    sWindow = [MainWindow alloc];
    return sWindow;
}
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag{
    self=[super initWithContentRect:contentRect styleMask:style backing:backingStoreType defer:flag];
    [self setTitle:@"OSXPlayer"];
    NSButton*closeButton=[self standardWindowButton:NSWindowCloseButton];
    [closeButton setTarget:self];
    [closeButton setAction:@selector(closeThisWindow)];
    [self makeKeyAndOrderFront:self];
    [self makeMainWindow];
    [self setAcceptsMouseMovedEvents:YES];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:self];

    return self;
}
-(void)windowDidResize:(NSNotification *)notification{
    NSRect rect=[[AliceView Instance] frame];
    NSRect adapted_rect=[[AliceView Instance] convertRectFromBacking:rect];
    NSLog(@"osx player window did resize");
}
-(void)closeThisWindow {
    [self close];
    exit(0);
}
@end
