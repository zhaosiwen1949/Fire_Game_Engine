//
//  NotificationManager.h
//  iPhonePlayer
//
//  Created by heck on 2018/8/28.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>

@interface NotificationManager : NSObject<UNUserNotificationCenterDelegate>
-(id)init;
-(void)Export;
-(void)cancelAllNotification;
-(void)scheduleNotification:(const char*)notification_id title:(const char*)title_text body:(const char*)body_text delay:(int) delay_time;
+(NotificationManager*)getInstance;
@end
