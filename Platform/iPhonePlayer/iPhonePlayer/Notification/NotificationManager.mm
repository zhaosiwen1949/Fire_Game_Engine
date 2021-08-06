//
//  NotificationManager.m
//  iPhonePlayer
//
//  Created by heck on 2018/8/28.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import "NotificationManager.h"
#include "Runtime/Debugger/Log.h"
static NotificationManager *sNotificationManager=nullptr;
static char *sDeviceToken=nullptr;
extern "C" void SetDeviceToken(const void*token,int len){
    sDeviceToken=new char[len+1];
    memset(sDeviceToken, 0, len+1);
    memcpy(sDeviceToken, token, len);
}
@implementation NotificationManager
+(NotificationManager*)getInstance{
    if (nullptr==sNotificationManager) {
        sNotificationManager=[[NotificationManager alloc]init];
    }
    return sNotificationManager;
}
-(id)init{
    [super init];
    float ios_version=[[UIDevice currentDevice].systemVersion doubleValue];
    if(ios_version>=8.0){
        if(ios_version>=10.0){
            UNUserNotificationCenter *center=[UNUserNotificationCenter currentNotificationCenter];
            center.delegate=self;
            UNAuthorizationOptions options=UNAuthorizationOptionBadge|UNAuthorizationOptionAlert|UNAuthorizationOptionSound;
            [center requestAuthorizationWithOptions:options completionHandler:^(BOOL granted,NSError*_Nullable error){
                if (granted) {
                    NSLog(@"register sucess");
                }
            }];
        }else{
            UIUserNotificationSettings*settings=[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge|UIUserNotificationTypeAlert|UIUserNotificationTypeSound categories:nil];
            [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
        }
    }
    return self;
}
-(void)cancelAllNotification{
    NSLog(@"cancel all notification");
    [UIApplication sharedApplication].applicationIconBadgeNumber=0;
    [[UIApplication sharedApplication]cancelAllLocalNotifications];
}
-(void)scheduleNotification:(const char*)notification_id title:(const char*)title_text body:(const char*)body_text delay:(int) delay_time{
    NSDictionary *infoDic = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:notification_id],@"id", nil];
    UILocalNotification *notification=[[UILocalNotification alloc]init];
    notification.userInfo = infoDic;
    notification.alertTitle=[NSString stringWithUTF8String:title_text];
    notification.alertBody=[NSString stringWithUTF8String:body_text];
    notification.soundName=UILocalNotificationDefaultSoundName;
    notification.fireDate=[NSDate dateWithTimeIntervalSinceNow:delay_time];
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
}
#pragma un user notification center delegate
-(void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler{
    
}
-(void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler{
    completionHandler(UNNotificationPresentationOptionBadge|UNNotificationPresentationOptionSound|UNNotificationPresentationOptionAlert);
}
@end

