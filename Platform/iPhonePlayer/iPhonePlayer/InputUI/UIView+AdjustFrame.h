    //
//  UIView+AdjustFrame.h
//  xib练习
//
//  Created by LOLITA on 17/6/14.
//  Copyright © 2017年 LOLITA. All rights reserved.
//

#import <UIKit/UIKit.h>
#define kAdjust(a) [UIScreen mainScreen].bounds.size.width/375.0*a
@interface UIView (AdjustFrame)
/*---------------------Frame---------------------*/
/**
 View起始位置
 */
@property (nonatomic) CGPoint viewOrigin;
/**
 View尺寸
 */
@property (nonatomic) CGSize viewSize;
/*---------------------Frame Origin---------------------*/
/**
 origin.x
 */
@property (nonatomic) CGFloat x;
/**
 origin.y
 */
@property (nonatomic) CGFloat y;
/*---------------------Frame Size---------------------*/
/**
 size.width
 */
@property (nonatomic) CGFloat viewWidth;
/**
 size.height
 */
@property (nonatomic) CGFloat viewHeight;
/*---------------------Frame Borders---------------------*/
/**
 顶部
 */
@property (nonatomic) CGFloat top;
/**
 左边
 */
@property (nonatomic) CGFloat left;
/**
 底部
 */
@property (nonatomic) CGFloat bottom;
/**
 右边
 */
@property (nonatomic) CGFloat right;
/*---------------------Center Point---------------------*/
/**
 center.x
 */
@property (nonatomic) CGFloat centerX;
/**
 center.y
 */
@property (nonatomic) CGFloat centerY;
/*---------------------Middle Point---------------------*/
/**
 本身坐标的中心点
 */
@property (nonatomic, readonly) CGPoint middlePoint;
/**
 本身坐标的中心x
 */
@property (nonatomic, readonly) CGFloat middleX;
/**
 本身坐标的y
 */
@property (nonatomic, readonly) CGFloat middleY;
@end
