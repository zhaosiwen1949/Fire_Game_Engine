//
//  EduCourseView.m
//  XEdu
//
//  Created by Heck on 2017/7/17.
//  Copyright © 2017年 battlefire. All rights reserved.
//
#import "MainViewInput.h"
#import "Utils.h"
#include"Runtime/RuntimePrefix.h"
#include"Runtime/Render/AliceGL.h"
static bool sIMEInputMode=false;
static bool sIMEInputModeDelay=false;
@implementation AliceView(Input)
-(void)initInputContext{
    backingStore = [[NSTextStorage alloc] init];
    NSMutableParagraphStyle *paragraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    [paragraphStyle setAlignment:NSCenterTextAlignment];
    NSFont *font = [NSFont systemFontOfSize:1.0f];
    
    defaultAttributes = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
                         paragraphStyle, NSParagraphStyleAttributeName,
                         font, NSFontAttributeName,
                         nil];
    markedAttributes = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
                        paragraphStyle, NSParagraphStyleAttributeName,
                        font, NSFontAttributeName,
                        [NSColor lightGrayColor], NSForegroundColorAttributeName,
                        nil];
}
- (void)keyDown:(NSEvent *)event{
    char c=[[event characters] UTF8String][0];
    if(c=='v'&&([event modifierFlags]&NSDeviceIndependentModifierFlagsMask)==NSCommandKeyMask){//ctrl+v
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSArray *classes = [[NSArray alloc] initWithObjects:[NSString class], nil];
        NSDictionary *options = [NSDictionary dictionary];
        NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
        if (copiedItems != nil) {
            
        }
        return ;
    }
    if(sIMEInputMode==false){
    }
    [self interpretKeyEvents:[NSArray arrayWithObject:event]];
}

- (void)keyUp:(NSEvent *)event{
    if(sIMEInputMode==false){
    }
    if(sIMEInputModeDelay==false){
        sIMEInputMode=false;
    }
}
- (void)mouseDown:(NSEvent *)event
{
    if([self EnableEvent]){
        
    }
}
-(void)rightMouseDown:(NSEvent *)event{
    if([self EnableEvent]){
        
    }
}
-(void)rightMouseUp:(NSEvent *)event{
    if([self EnableEvent]){
        
    }
}
-(void)rightMouseDragged:(NSEvent *)event{
    if([self EnableEvent]){
        
    }
}

-(void)mouseDragged:(NSEvent *)event{
    if([self EnableEvent]){
        
    }
}
-(void)mouseMoved:(NSEvent *)event{
    if([self EnableEvent]){
        
    }
}

- (void)mouseUp:(NSEvent *)event
{
    if([self EnableEvent]){
        
    }
}

- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange {
    sIMEInputModeDelay=false;
    const char * str=[aString UTF8String];
    int len=strlen(str);
    if(len>1){
        
    }else{
        
    }
    return ;
}
- (void)setMarkedText:(id)aString selectedRange:(NSRange)newSelection replacementRange:(NSRange)replacementRange {
    sIMEInputMode=true;
    sIMEInputModeDelay=true;
    if (replacementRange.location == NSNotFound) {
        if (markedRange.location != NSNotFound) {
            replacementRange = markedRange;
        } else {
            replacementRange = selectedRange;
        }
    }
    // Add the text
    [backingStore beginEditing];
    if ([aString length] == 0) {
        [backingStore deleteCharactersInRange:replacementRange];
        [self unmarkText];
    } else {
        markedRange = NSMakeRange(replacementRange.location, [aString length]);
        if ([aString isKindOfClass:[NSAttributedString class]]) {
            [backingStore replaceCharactersInRange:replacementRange withAttributedString:aString];
        } else {
            [backingStore replaceCharactersInRange:replacementRange withString:aString];
        }
        [backingStore addAttributes:markedAttributes range:markedRange];
    }
    [backingStore endEditing];
    
    // Redisplay
    selectedRange.location = replacementRange.location + newSelection.location; // Just for now, only select the marked text
    selectedRange.length = newSelection.length;
    [[self inputContext] invalidateCharacterCoordinates]; // recentering
}

- (void)unmarkText {
    //NSLog(@"unmarkText %@",markedRange);
    markedRange = NSMakeRange(NSNotFound, 0);
    [[self inputContext] discardMarkedText];
}

- (NSRange)selectedRange {
    //NSLog(@"selectedRange %@",markedRange);
    return selectedRange;
}
- (NSRange)markedRange {
    //NSLog(@"markedRange %@",markedRange);
    return markedRange;
}

- (BOOL)hasMarkedText {
    //NSLog(@"hasMarkedText %@",markedRange);
    return (markedRange.location == NSNotFound ? NO : YES);
}

- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    return [backingStore attributedSubstringFromRange:aRange];
}

- (NSArray *)validAttributesForMarkedText {
    return [NSArray arrayWithObjects:NSMarkedClauseSegmentAttributeName, NSGlyphInfoAttributeName, nil];
}

- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    return NSMakeRect(0, 0, 0, 0);
}

- (NSUInteger)characterIndexForPoint:(NSPoint)aPoint {
    return 0;
}
- (NSAttributedString *)attributedString {
    // This method is optional, but our backing store is an attributed string anyway
    return backingStore;
}
- (NSInteger)windowLevel {
    // This method is optional but easy to implement
    return [[self window] level];
}
- (CGFloat)fractionOfDistanceThroughGlyphForPoint:(NSPoint)aPoint {
    return 0.5f;
}
- (CGFloat)baselineDeltaForCharacterAtIndex:(NSUInteger)anIndex {
    return 0.0f;
}
- (void)setStringValue:(NSString *)aString {
    [backingStore beginEditing];
    [backingStore replaceCharactersInRange:NSMakeRange(0, [backingStore length]) withString:aString];
    [backingStore setAttributes:defaultAttributes range:NSMakeRange(0, [aString length])];
    [backingStore endEditing];
    [self unmarkText];
    selectedRange = NSMakeRange([aString length], 0);
    [[self inputContext] invalidateCharacterCoordinates];
    //[self setNeedsDisplay:YES];
}
@end
