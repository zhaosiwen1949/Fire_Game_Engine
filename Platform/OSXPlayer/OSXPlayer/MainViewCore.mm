//
//  MainViewAPI.m
//  东汉书院
//
//  Created by Heck on 2018/6/24.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import "MainViewCore.h"
#import "MainWindow.h"
#import "Utils.h"
#import "MNMBill.h"
#include"Runtime/RuntimePrefix.h"
#include"Runtime/Render/AliceGL.h"
static bool sPermenantDisable=false;
static bool sEnableEvent=true;
static AliceView * sCourseView=nil;
@implementation AliceView(Core)
- (BOOL)acceptsFirstResponder {
    return YES;
}
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
    NSPasteboard *pboard;
    NSDragOperation sourceDragMask;
    
    sourceDragMask = [sender draggingSourceOperationMask];
    pboard = [sender draggingPasteboard];
    
    if ( [[pboard types] containsObject:NSFilenamesPboardType] ) {
        NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
        NSString*filePath=(NSString*)[files objectAtIndex:0];
    }
    return YES;
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
    NSPasteboard *pboard;
    NSDragOperation sourceDragMask;
    
    sourceDragMask = [sender draggingSourceOperationMask];
    pboard = [sender draggingPasteboard];
    
    if ( [[pboard types] containsObject:NSFilenamesPboardType] ) {
        if (sourceDragMask & NSDragOperationLink) {
            return NSDragOperationLink;
        } else if (sourceDragMask & NSDragOperationCopy) {
            return NSDragOperationCopy;
        }
    }
    return NSDragOperationNone;
}
-(void)InitAlice{
    [self initInputContext];
}
-(void)EnterApp{
    sCourseView=self;
    [NSTimer scheduledTimerWithTimeInterval:0.016 target:self selector:@selector(renderOneFrame) userInfo:nil repeats:YES];
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [self registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];
    CGRect rect=[sCourseView frame];
    CGRect adapted_rect=[sCourseView convertRectToBacking:rect];
    NSScreen *mainScreen=[NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    NSRect desktopRect = [mainScreen frame];
    float screenWidth=screenRect.size.width;
    float screenHeight=screenRect.size.height;
    float scale=adapted_rect.size.width/rect.size.width;
}
-(BOOL)EnableEvent{
    return sEnableEvent;
}
-(void)renderOneFrame{
    [[self openGLContext] makeCurrentContext];
    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    [[self openGLContext]flushBuffer];
}
@end
