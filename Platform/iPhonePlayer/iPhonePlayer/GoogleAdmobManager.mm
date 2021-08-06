//
//  GoogleAdmobManager.m
//  iPhonePlayer
//
//  Created by heck on 2018/9/10.
//  Copyright © 2018年 battlefire. All rights reserved.
//
#import "GoogleAdmobManager.h"
#include "Runtime/Debugger/Log.h"
static GoogleAdmobManager *sGoogleAdmobManager=nullptr;
static NSString*sTestDevice=nullptr;
#pragma invoke method in mainthread
@implementation GoogleAdmobManager
+(GoogleAdmobManager*)getInstance{
    if(sGoogleAdmobManager==nullptr){
        sGoogleAdmobManager=[[GoogleAdmobManager alloc]init];
    }
    return sGoogleAdmobManager;
}
-(void)LoadBannerView:(const char*)unit_id{
    if (self.mBannerView!=nullptr) {
        [self.mBannerView release];
    }
    self.mBannerView=[[GADBannerView alloc]initWithAdSize:kGADAdSizeBanner];
    self.mBannerView.translatesAutoresizingMaskIntoConstraints = NO;
    [self.mRootViewController.view addSubview: self.mBannerView];
    [self.mRootViewController.view addConstraints:@[[NSLayoutConstraint constraintWithItem:self.mBannerView
                                                             attribute:NSLayoutAttributeBottom
                                                             relatedBy:NSLayoutRelationEqual
                                                                toItem:self.mRootViewController.bottomLayoutGuide
                                                             attribute:NSLayoutAttributeTop
                                                            multiplier:1
                                                              constant:0],
                                [NSLayoutConstraint constraintWithItem:self.mBannerView
                                                             attribute:NSLayoutAttributeCenterX
                                                             relatedBy:NSLayoutRelationEqual
                                                                toItem:self.mRootViewController.view
                                                             attribute:NSLayoutAttributeCenterX
                                                            multiplier:1
                                                              constant:0]]];
    self.mBannerView.rootViewController = self.mRootViewController;
    GADRequest *request = [GADRequest request];
    if(sTestDevice!=nullptr){
        request.testDevices=@[sTestDevice];
    }
    [self.mBannerView setAdUnitID:[NSString stringWithUTF8String:unit_id]];
    [self.mBannerView loadRequest:request];
}
-(void)LoadInterstitialView:(const char*)unit_id{
    GADRequest *request = [GADRequest request];
    if(sTestDevice!=nullptr){
        request.testDevices=@[sTestDevice];
    }
    if(self.mInterstitial!=nullptr){
        [self.mInterstitial release];
    }
    self.mInterstitial= [[GADInterstitial alloc]initWithAdUnitID:[NSString stringWithUTF8String:unit_id]];
    self.mInterstitial.delegate=self;
    [self.mInterstitial loadRequest:request];
}
-(void)ShowInterstitialView{
    if([self.mInterstitial isReady]){
        [self.mInterstitial presentFromRootViewController:self.mRootViewController];
    }
}
-(void)ShowRewardVideo{
    if([[GADRewardBasedVideoAd sharedInstance] isReady]){
        [[GADRewardBasedVideoAd sharedInstance] presentFromRootViewController:self.mRootViewController];
    }
}
#pragma mark Ad Request Lifecycle Notifications
/// Tells the delegate that an ad request successfully received an ad. The delegate may want to add
/// the banner view to the view hierarchy if it hasn't been added yet.
- (void)adViewDidReceiveAd:(GADBannerView *)bannerView{
    
}

/// Tells the delegate that an ad request failed. The failure is normally due to network
/// connectivity or ad availablility (i.e., no fill).
- (void)adView:(GADBannerView *)bannerView didFailToReceiveAdWithError:(GADRequestError *)error{
    
}

#pragma mark Click-Time Lifecycle Notifications

/// Tells the delegate that a full screen view will be presented in response to the user clicking on
/// an ad. The delegate may want to pause animations and time sensitive interactions.
- (void)adViewWillPresentScreen:(GADBannerView *)bannerView{
    
}

/// Tells the delegate that the full screen view will be dismissed.
- (void)adViewWillDismissScreen:(GADBannerView *)bannerView{
    
}

/// Tells the delegate that the full screen view has been dismissed. The delegate should restart
/// anything paused while handling adViewWillPresentScreen:.
- (void)adViewDidDismissScreen:(GADBannerView *)bannerView{
    
}

/// Tells the delegate that the user click will open another app, backgrounding the current
/// application. The standard UIApplicationDelegate methods, like applicationDidEnterBackground:,
/// are called immediately before this method is called.
- (void)adViewWillLeaveApplication:(GADBannerView *)bannerView{
    
}
#pragma mark GADRewardBasedVideoAdDelegate implementation
- (void)rewardBasedVideoAdDidReceiveAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad is received.");
}
- (void)rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Opened reward based video ad.");
}
- (void)rewardBasedVideoAdDidStartPlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad started playing.");
}
- (void)rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad is closed.");
}
- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd
   didRewardUserWithReward:(GADAdReward *)reward {
    
}
- (void)rewardBasedVideoAdWillLeaveApplication:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad will leave application.");
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didFailToLoadWithError:(NSError *)error {
    NSLog(@"Reward based video ad failed to load.");
}
#pragma mark GADInterstitialDelegate implementation
/// Tells the delegate an ad request succeeded.
- (void)interstitialDidReceiveAd:(GADInterstitial *)ad {
    NSLog(@"interstitialDidReceiveAd");
}
/// Tells the delegate an ad request failed.
- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error {
    NSLog(@"interstitial:didFailToReceiveAdWithError: %@", [error localizedDescription]);
}

/// Tells the delegate that an interstitial will be presented.
- (void)interstitialWillPresentScreen:(GADInterstitial *)ad {
    NSLog(@"interstitialWillPresentScreen");
}
-(void)interstitialDidFailToPresentScreen:(GADInterstitial *)ad{
    
}
/// Tells the delegate the interstitial is to be animated off the screen.
- (void)interstitialWillDismissScreen:(GADInterstitial *)ad {
    NSLog(@"interstitialWillDismissScreen");
}

/// Tells the delegate the interstitial had been animated off the screen.
- (void)interstitialDidDismissScreen:(GADInterstitial *)ad {
    NSLog(@"interstitialDidDismissScreen");
}

/// Tells the delegate that a user click will open another app
/// (such as the App Store), backgrounding the current app.
- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad {
    NSLog(@"interstitialWillLeaveApplication");
}
@end
