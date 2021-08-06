//
//  AliceGlobalState.h
//  iPhonePlayer
//
//  Created by Heck on 2018/6/21.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AliceGlobalState : NSObject
+(BOOL)IsNavBarHidden;
+(void)ShowNavBar:(BOOL)show;
@end
