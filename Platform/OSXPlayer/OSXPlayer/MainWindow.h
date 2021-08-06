//
//  MainWindow.h
//  东汉书院
//
//  Created by heck on 2018/12/11.
//  Copyright © 2018 battlefire. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface MainWindow : NSWindow<NSWindowDelegate>
+(MainWindow*)Instance;
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag;
@end
NS_ASSUME_NONNULL_END
