//
//  Scan_VC.h
//  仿支付宝
//
//  Created by 张国兵 on 15/12/9.
//  Copyright © 2015年 zhangguobing. All rights reserved.
//

#import <UIKit/UIKit.h>
@interface QRCodeScanViewController : UIViewController
@property (nonatomic, assign) CGRect ScanFieldRect;
@property (nonatomic, assign) CGRect TipTextRect;
-(void)initWithRect:(CGRect)scan_view_rect tip_rect:(CGRect)tip_text_rect;
-(id)initWithTip:(const char*)str;
@end
