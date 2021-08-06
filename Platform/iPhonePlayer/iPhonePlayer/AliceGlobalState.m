//
//  AliceGlobalState.m
//  iPhonePlayer
//
//  Created by Heck on 2018/6/21.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import "AliceGlobalState.h"
static BOOL sIsNavBarHidden=YES;
@implementation AliceGlobalState
+(BOOL)IsNavBarHidden{
    return sIsNavBarHidden;
}
+(void)ShowNavBar:(BOOL)show{
    sIsNavBarHidden=(NO==show);
}
@end
