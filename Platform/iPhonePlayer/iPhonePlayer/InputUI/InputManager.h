//
//  InputManager.h
//  iPhonePlayer
//
//  Created by heck on 2018/8/24.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InputView.h"
@interface InputManager : NSObject<InputViewDelegate>
@property (strong ,nonatomic) InputView *inputView;
-(id)initWithParentView:(UIView*)parent;
-(void)ShowKeyboard:(const char*)utf8Str;
@end
