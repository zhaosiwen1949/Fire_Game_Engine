//
//  AliceView.h
//  东汉书院
//
//  Created by Heck on 2018/6/24.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#import <CoreVideo/CVDisplayLink.h>

@interface AliceView : NSOpenGLView{
    NSTextStorage*backingStore;
    NSMutableDictionary *defaultAttributes;
    NSMutableDictionary *markedAttributes;
    NSRange markedRange;
    NSRange selectedRange;
}
+(AliceView*)Instance;
-(void)InitRenderView:(NSRect)rect;
-(void)InitAlice;
-(void)EnterApp;
@end
