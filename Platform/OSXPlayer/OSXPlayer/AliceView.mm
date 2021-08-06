//
//  AliceView.m
//  东汉书院
//
//  Created by Heck on 2018/6/24.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import "AliceView.h"
static AliceView*sAliceView=nullptr;
@interface AliceView ()
@end
@implementation AliceView
+(AliceView*)Instance{
    if(sAliceView!=nullptr){
        return sAliceView;
    }
    sAliceView=[AliceView alloc];
    return sAliceView;
}
-(void)InitRenderView:(NSRect)rect{
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] ={
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAStencilSize,8,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADoubleBuffer ,
        NSOpenGLPFAAccelerated  ,
        NSOpenGLPFANoRecovery   ,
        0
    };
    NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes] autorelease];
    [[self initWithFrame:rect pixelFormat:pixelFormat]retain];
}
-(void)prepareOpenGL{
    [self setWantsBestResolutionOpenGLSurface:YES];
    [[self openGLContext] makeCurrentContext];
    NSLog(@"gl version %s ,glsl version %s",glGetString(GL_VERSION),glGetString(GL_SHADING_LANGUAGE_VERSION));
    [self InitAlice];
    [self EnterApp];
}
@end
