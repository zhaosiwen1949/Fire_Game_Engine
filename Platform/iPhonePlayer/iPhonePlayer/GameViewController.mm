//
//  GameViewController.m
//  iPhonePlayer
//
//  Created by Heck on 16/5/17.
//  Copyright © 2016年 battlefire. All rights reserved.
//

#import "GameViewController.h"
#import "QRCodeScanViewController.h"
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/CADisplayLink.h>
#import "MNMBill.h"
#import "Utils.h"
#import "AliceGlobalState.h"
#import "InputUI/InputManager.h"
#import "GoogleAdmobManager.h"
#import "NotificationManager.h"
#include "Runtime/RuntimePrefix.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/machine.h>

extern "C" void Render();
static bool sPermenantDisable=false;
static bool sEnableEvent=true;
static std::unordered_map<void*,CGPoint*> mTouches;
static GameViewController *sGameViewController;
UIInterfaceOrientationMask CurrentOrientationMask=UIInterfaceOrientationMaskPortrait;

CADisplayLink *displayLink;
CVReturn displayCallback(CADisplayLink * displayLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext);

CVReturn displayCallback(CADisplayLink * displayLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext)
{
    @autoreleasepool {
        GameViewController*view = (GameViewController *)displayLinkContext;
        [view renderForTime:*inOutputTime];
    }
    return kCVReturnSuccess;
}
extern "C" UIViewController* InitGameViewController(void*param)
{
    sGameViewController=[[GameViewController alloc]init];
    return sGameViewController;
}


extern "C" void ShowiPhoneSoftKeyboard(const char *utf8Str){
    [sGameViewController ShowKeyboard:utf8Str];
}

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

@interface GameViewController ()
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) InputManager *mInputManager;
- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if(false==sEnableEvent){
        return ;
    }
    if ([touches count]>0) {
        for (UITouch* touch in touches) {
            auto iter=mTouches.find(touch);
            if (iter==mTouches.end()) {
                CGPoint *point=new CGPoint;
                *point=[touch locationInView:self.view];
                mTouches.insert(std::pair<void*, CGPoint*>(touch,point));
            }
        }
    }
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if(false==sEnableEvent){
        return ;
    }
    if ([touches count]>0) {
        for (UITouch* touch in touches) {
            auto iter=mTouches.find(touch);
            if (iter!=mTouches.end()) {
                CGPoint*point=iter->second;
                CGPoint pos=[touch locationInView:self.view];
                if (pos.x!=point->x||
                    pos.y!=point->y)
                {
                    CGPoint*point=iter->second;
                    *point=pos;
                }
            }
        }
    }
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if(false==sEnableEvent){
        return ;
    }
    if ([touches count]>0) {
        for (UITouch* touch in touches) {
            auto iter=mTouches.find(touch);
            if (iter!=mTouches.end()) {
                CGPoint*point=iter->second;
                *point=[touch locationInView:self.view];
                delete iter->second;
                mTouches.erase(iter);
            }
        }
    }
}

-(void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    if(false==sEnableEvent){
        return ;
    }
    if ([touches count]>0) {
        for (UITouch* touch in touches) {
            auto iter=mTouches.find(touch);
            if (iter!=mTouches.end()) {
                CGPoint*point=iter->second;
                *point=[touch locationInView:self.view];
                delete iter->second;
                mTouches.erase(iter);
            }
        }
    }
}
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event{
    if (motion == UIEventSubtypeMotionShake) {
    
    }else{
    }
}

-(void)OrientChanged:(NSNotification*)noti{
    int fullresolutionwidth,fullresolutionheight;
    UIDeviceOrientation orientation=[UIDevice currentDevice].orientation;
    CGRect rect=[[UIScreen mainScreen] nativeBounds];
    if (orientation==UIDeviceOrientationPortrait||
        orientation==UIDeviceOrientationPortraitUpsideDown) {
        fullresolutionwidth=(int)(rect.size.width);
        fullresolutionheight=(int)(rect.size.height);
    }else{
        fullresolutionwidth=(int)(rect.size.height);
        fullresolutionheight=(int)(rect.size.width);
    }
}

-(void)keyboardWillShow:(NSNotification*)notification
{
    //NSDictionary *info=[notification userInfo];
    //CGSize kbSize=[[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
}
-(void)keyboardWillHide:(NSNotification*)notification
{
    
}
-(void)OnEditEnd:(UITextField*)tf
{
    
}
-(void)ShowKeyboard:(const char*)utf8Str
{
    [_mInputManager ShowKeyboard:utf8Str];
}
-(BOOL)shouldAutorotate {
    return YES;
}
-(UIInterfaceOrientationMask)supportedInterfaceOrientations{
    //NSLog(@"supportedInterfaceOrientations");
    return CurrentOrientationMask;
}
-(void)viewDidAppear:(BOOL)animated{
    //NSLog(@"viewDidAppear");
    self.navigationController.navigationBarHidden=[AliceGlobalState IsNavBarHidden];
    static bool inited=false;
    if(!inited){
        inited=true;
    }
}
-(void)viewDidDisappear:(BOOL)animated{
    self.navigationController.navigationBarHidden=[AliceGlobalState IsNavBarHidden];
}
- (NSString *)getCPUType {
    NSMutableString *cpu = [[NSMutableString alloc] init];
    size_t size;
    cpu_type_t type;
    cpu_subtype_t subtype;
    size = sizeof(type);
    sysctlbyname("hw.cputype", &type, &size, NULL, 0);
    
    size = sizeof(subtype);
    sysctlbyname("hw.cpusubtype", &subtype, &size, NULL, 0);
    
    // values for cputype and cpusubtype defined in mach/machine.h
    if (type == CPU_TYPE_X86_64) {
        [cpu appendString:@"x86_64"];
    } else if (type == CPU_TYPE_X86) {
        [cpu appendString:@"x86"];
    } else if (type == CPU_TYPE_ARM) {
        [cpu appendString:@"ARM"];
        switch(subtype)
        {
            case CPU_SUBTYPE_ARM_V6:
                [cpu appendString:@"V6"];
                break;
            case CPU_SUBTYPE_ARM_V7:
                [cpu appendString:@"V7"];
                break;
            case CPU_SUBTYPE_ARM_V8:
                [cpu appendString:@"V8"];
                break;
        }
    }
    return cpu;
}
- (void)viewDidLoad{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [self.view setMultipleTouchEnabled:YES];
    [self setupGL];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(OrientChanged:) name:UIApplicationDidChangeStatusBarOrientationNotification object:nil];
    _mInputManager=[[[InputManager alloc] initWithParentView:self.view]retain];
}

- (void)dealloc{
    [self tearDownGL];
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    [super dealloc];
}
- (void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];
}
- (BOOL)prefersStatusBarHidden {
    return YES;
}
- (void)setupUpdater{
    // Create a display link capable of being used with all active displays
    CADisplayLink*timer=[CADisplayLink displayLinkWithTarget:self selector:@selector(renderForTime)];
    [timer addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    NSLog(@"gl version %s ,glsl version %s",glGetString(GL_VERSION),glGetString(GL_SHADING_LANGUAGE_VERSION));
}
- (void)setupGL{
    [EAGLContext setCurrentContext:self.context];
    [self OrientChanged:nil];
    [[NotificationManager getInstance] cancelAllNotification];
    [GoogleAdmobManager getInstance];
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    [self setPreferredFramesPerSecond:60];
}
- (void)tearDownGL{
    [EAGLContext setCurrentContext:nil];
}
#pragma mark - GLKView and GLKViewController delegate methods
- (void)update{
    //NSLog(@"glkView update");
}
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
- (void)renderForTime{
    [EAGLContext setCurrentContext:self.context];
}
- (void)qb_imagePickerController:(QBImagePickerController *)imagePickerController didFinishPickingAssets:(NSArray *)assets
{
    NSLog(@"Selected assets:");
    PHAsset*asset=assets[0];
    PHImageManager *manager = [PHImageManager defaultManager];
    PHImageRequestOptions *options = [PHImageRequestOptions new];
    options.synchronous = YES;
    options.deliveryMode = PHImageRequestOptionsDeliveryModeHighQualityFormat;
    options.resizeMode = PHImageRequestOptionsResizeModeNone;
    options.networkAccessAllowed = NO;
    
    [manager requestImageForAsset:asset targetSize:PHImageManagerMaximumSize contentMode:PHImageContentModeDefault options:options resultHandler:^(UIImage *resultImage, NSDictionary *info){
    }];
    [self dismissViewControllerAnimated:YES completion:NULL];
    [imagePickerController release];
}

- (void)qb_imagePickerControllerDidCancel:(QBImagePickerController *)imagePickerController
{
    NSLog(@"Canceled.");
    [self dismissViewControllerAnimated:YES completion:NULL];
    [imagePickerController release];
}
@end
