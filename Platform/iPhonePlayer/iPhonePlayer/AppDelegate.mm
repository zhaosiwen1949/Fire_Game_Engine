//
//  AppDelegate.m
//  iPhonePlayer
//
//  Created by Heck on 16/5/17.
//  Copyright © 2016年 battlefire. All rights reserved.
//

#import "AppDelegate.h"
#import "GameViewController.h"
#import "AliceGlobalState.h"
#import "NotificationManager.h"
extern "C" UIViewController* InitGameViewController(void*param);
extern UIInterfaceOrientationMask CurrentOrientationMask;
@interface AppDelegate ()
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [UIApplication sharedApplication].statusBarOrientation=UIInterfaceOrientationPortrait;
    id alice_view_controller=InitGameViewController(nullptr);
    [alice_view_controller retain];
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    UINavigationController*nc=[[UINavigationController alloc]initWithRootViewController:alice_view_controller];
    [AliceGlobalState ShowNavBar:NO];
    ((UIViewController*)alice_view_controller).navigationController.navigationBarHidden=YES;
    [self.window setRootViewController:nc];
    [self.window makeKeyAndVisible];
    [[UIApplication sharedApplication] registerForRemoteNotifications];
    return YES;
}

-(void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification{
    [[NotificationManager getInstance]cancelAllNotification];
}
// 注册成功回调方法，其中deviceToken即为APNs返回的token
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    NSString *tokenStr=[[[[deviceToken description]
                           stringByReplacingOccurrencesOfString: @"<" withString: @""]
                          stringByReplacingOccurrencesOfString: @">" withString: @""]
                         stringByReplacingOccurrencesOfString: @" " withString: @""];
    NSLog(@"device token : %@",tokenStr);
    NSLog(@"device token : %@",deviceToken);
}
// 注册失败回调方法，处理失败情况
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    
}
/**
 iOS3.0 ~ 10.0
 
 前台状态下,当接收到远程通知
 点击通知
 完全退出情况下，点击通知，不会执行
 */
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    
    double version = [[[UIDevice currentDevice] systemVersion] doubleValue];
    if (version < 10.0) {
    }
    NSLog(@"收到远程通知 %f",version);
}

/*
 iOS7.0 ~ 10.0
 
 如果实现了该方法，则不执行application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
 
 前台状态下,当接收到远程通知
 点击通知
 完全退出情况下，点击通知，也会执行
 
 只要接收到了通知，不管当前APP的状态，不管后台，还是锁屏，都直接执行这个方法
 要求：
 1.必须勾选后台模式Remote Notification
 2.告诉系统是否有新的内容更新（执行完成代码块）
 3.设置发送通知的格式（"content-available":"xxx"）
 {
 "aps" : {
 "alert":"This is some fancy message.",
 "badge":1,
 "content-available":"xxx"
 }
 }
 
 调用系统回调代码块的作用
 1.系统会估量App消耗的电量，并根据传递的UIBackgroundFetchResult参数记录新数据是否可用
 2.调用完成的处理代码时，应用的界面缩略图会自动更新
 
 */
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    double version = [[[UIDevice currentDevice] systemVersion] doubleValue];
    if (version < 10.0) {
    }
    NSLog(@"收到远程通知-----2 %f",version);
    completionHandler(UIBackgroundFetchResultNewData);
}

/*
 前台状态下
 完全退出，点击通知，不会执行这个方法
 */
- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {
    if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
        NSLog(@"收到远程通知-----3.remote");
    }
    else {
        NSLog(@"收到本地通知-----3.local");
    }
    // UNNotificationPresentationOptionAlert 在前台状态下会出现通知栏
    completionHandler(UNNotificationPresentationOptionSound | UNNotificationPresentationOptionBadge | UNNotificationPresentationOptionAlert);
}

/*
 点击通知
 完全退出，点击通知，也会执行这个方法
 */
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)())completionHandler {
    if([response.notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
        NSLog(@"收到远程通知-----4.remote");
    }
    else {
        NSLog(@"收到本地通知-----4.local");
    }
    completionHandler();
}
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window{
    return CurrentOrientationMask;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    NSLog(@"applicationWillEnterForeground");
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    NSLog(@"applicationDidBecomeActive");
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    NSLog(@"applicationWillTerminate");
}

@end
