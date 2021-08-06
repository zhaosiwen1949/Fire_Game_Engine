//
//  GoogleAdmobManager.h
//  iPhonePlayer
//
//  Created by heck on 2018/9/10.
//  Copyright © 2018年 battlefire. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GoogleMobileAds/GoogleMobileAds.h>
@interface GoogleAdmobManager : NSObject<GADRewardBasedVideoAdDelegate,GADInterstitialDelegate,GADBannerViewDelegate>
+(GoogleAdmobManager*)getInstance;
-(void)Export:(UIViewController*)rootViewController;
-(void)LoadBannerView:(const char*)unit_id;
-(void)LoadInterstitialView:(const char*)unit_id;
-(void)ShowInterstitialView;
-(void)ShowRewardVideo;
@property(nonatomic,strong)UIViewController*mRootViewController;
@property(nonatomic,strong)GADInterstitial*mInterstitial;
@property(nonatomic,strong)GADBannerView*mBannerView;
@end
