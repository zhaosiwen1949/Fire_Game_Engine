//
//  Scan_VC.m
//  仿支付宝
//
//  Created by 张国兵 on 15/12/9.
//  Copyright © 2015年 zhangguobing. All rights reserved.

#import "QRCodeScanViewController.h"
#import <AVFoundation/AVFoundation.h>
#import "AliceGlobalState.h"
extern UIInterfaceOrientationMask CurrentOrientationMask;
@interface QRCodeScanViewController()
    <UIAlertViewDelegate,
    AVCaptureMetadataOutputObjectsDelegate,
    UINavigationControllerDelegate,
    UIImagePickerControllerDelegate>
{
}
@property (nonatomic, strong) AVCaptureSession *session;
@property (nonatomic, strong)   UIView *maskView;
@property (nonatomic, strong) UIView *scanWindow;
@property (nonatomic, strong) UILabel *tipLabel;
@property (nonatomic, strong) UIImageView *scanNetImageView;
@end
@implementation QRCodeScanViewController
- (void)dealloc{
    [super dealloc];
}
- (BOOL)prefersStatusBarHidden {
    return YES;
}
-(void)viewWillAppear:(BOOL)animated{
    self.navigationController.navigationBarHidden=[AliceGlobalState IsNavBarHidden];
    [self resumeAnimation];
    
}
-(void)viewDidDisappear:(BOOL)animated{
    self.navigationController.navigationBarHidden=[AliceGlobalState IsNavBarHidden];
}
-(void)initWithRect:(CGRect)scan_view_rect tip_rect:(CGRect)tip_text_rect{
    self.ScanFieldRect=scan_view_rect;
    self.TipTextRect=tip_text_rect;
}
-(void)TryToInitScanFieldRect{
    if(self.ScanFieldRect.size.width!=0.0f){
        return;
    }
    if(CurrentOrientationMask==UIInterfaceOrientationMaskLandscapeRight){
        self.ScanFieldRect=CGRectMake(30, 100, self.view.frame.size.height-130, self.view.frame.size.height-130);
    }else{
        self.ScanFieldRect=CGRectMake(30, 100, self.view.frame.size.width-60, self.view.frame.size.width-60);
    }
}
-(void)TryToInitTipTextRect{
    if(self.TipTextRect.size.width!=0.0f){
        return;
    }
    if(CurrentOrientationMask==UIInterfaceOrientationMaskLandscapeRight){
        self.TipTextRect=CGRectMake(self.view.frame.size.height+20, self.view.frame.size.height/2.0, self.view.frame.size.width/2.0f-20.0f, 100);
    }else{
        self.TipTextRect=CGRectMake(0, self.view.frame.size.width+40.0f, self.view.frame.size.width, 100);
    }
}
-(void)initMasks{
    if(CurrentOrientationMask==UIInterfaceOrientationMaskLandscapeRight){
        GLfloat scan_field_width=self.ScanFieldRect.size.width;
        GLfloat left_margin=30.0f;
        GLfloat right_margin=self.view.frame.size.width-30-scan_field_width;
        GLfloat top_margin=100.0f;
        GLfloat bottom_margin=30.0f;
        //left mask
        UIView*mask=[[UIView alloc]initWithFrame:CGRectMake(0,
                                                            0,
                                                            left_margin,
                                                            self.view.frame.size.height)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //right mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(self.ScanFieldRect.origin.x+self.ScanFieldRect.size.width,
                                                                                 0,
                                                                                 right_margin,
                                                                                 self.view.frame.size.height)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //top mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(30.0f,
                                                     0,
                                                     scan_field_width,
                                                     top_margin)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //bottom mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(30.0f,
                                                     100.0f+scan_field_width,
                                                     scan_field_width,
                                                     bottom_margin)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
    }else{
        GLfloat scan_field_width=self.ScanFieldRect.size.width;
        GLfloat left_margin=30.0f;
        GLfloat right_margin=30.0f;
        GLfloat top_margin=100.0f;
        GLfloat bottom_margin=self.view.frame.size.height-scan_field_width;
        //left mask
        UIView*mask=[[UIView alloc]initWithFrame:CGRectMake(0,
                                                            0,
                                                            left_margin,
                                                            self.view.frame.size.height)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //right mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(self.ScanFieldRect.origin.x+self.ScanFieldRect.size.width,
                                                     0,
                                                     right_margin,
                                                     self.view.frame.size.height)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //top mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(left_margin,
                                                     0,
                                                     scan_field_width,
                                                     top_margin)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
        //bottom mask
        mask=[[UIView alloc]initWithFrame:CGRectMake(left_margin,
                                                     top_margin+scan_field_width,
                                                     scan_field_width,
                                                     bottom_margin)];
        mask.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
        [self.view addSubview:mask];
    }
}
- (void)viewDidLoad {
    //NSLog(@"scan vc viewdidload");
    [super viewDidLoad];
   
}
-(void)setupTipTitleView{
    UILabel * tipLabel = [[UILabel alloc] initWithFrame:self.TipTextRect];
    tipLabel.text = @"将取景框对准二维码，即可自动扫描";
    tipLabel.textColor = [UIColor whiteColor];
    if(UIInterfaceOrientationMaskPortrait==CurrentOrientationMask){
        tipLabel.textAlignment = NSTextAlignmentCenter;
    }else{
        tipLabel.textAlignment = NSTextAlignmentLeft;
    }
    tipLabel.lineBreakMode = NSLineBreakByWordWrapping;
    tipLabel.numberOfLines = 2;
    tipLabel.font=[UIFont systemFontOfSize:12];
    tipLabel.backgroundColor = [UIColor clearColor];
    [self.view addSubview:tipLabel];
    self.tipLabel=tipLabel;
}
-(id)initWithTip:(const char *)str{
    //这个属性必须打开否则返回的时候会出现黑边
    self.view.clipsToBounds=YES;
    [self TryToInitScanFieldRect];
    [self TryToInitTipTextRect];
    //1.遮罩
    [self initMasks];
    //2.下边栏
    //[self setupBottomBar];
    //3.提示文本
    [self setupTipTitleView];
    [self.tipLabel setText:[NSString stringWithUTF8String:str]];
    //4.顶部导航
    [self setupNavView];
    //5.扫描区域
    [self setupScanWindowView];
    //6.开始动画
    [self beginScanning];
    
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(resumeAnimation) name:@"EnterForeground" object:nil];
    return self;
}
-(void)setupNavView{
    //1.返回
    UIButton *backBtn=[UIButton buttonWithType:UIButtonTypeCustom];
    backBtn.frame = CGRectMake(20, 30, 25, 25);
    [backBtn setBackgroundImage:[UIImage imageNamed:@"qrcode_scan_titlebar_back_nor"] forState:UIControlStateNormal];
    backBtn.contentMode=UIViewContentModeScaleAspectFit;
    [backBtn addTarget:self action:@selector(disMiss) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:backBtn];
}

- (void)setupBottomBar

{
    //1.下边栏
    /*UIView *bottomBar = [[UIView alloc] initWithFrame:CGRectMake(0, self.view.sd_height * 0.9, self.view.sd_width, self.view.sd_height * 0.1)];
    bottomBar.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.8];
    
    [self.view addSubview:bottomBar];
    
    //2.我的二维码
    UIButton * myCodeBtn=[UIButton buttonWithType:UIButtonTypeCustom];
    myCodeBtn.frame = CGRectMake(0,0, self.view.sd_height * 0.1*35/49, self.view.sd_height * 0.1);
    myCodeBtn.center=CGPointMake(self.view.sd_width/2, self.view.sd_height * 0.1/2);
    [myCodeBtn setImage:[UIImage imageNamed:@"qrcode_scan_btn_myqrcode_down"] forState:UIControlStateNormal];
    myCodeBtn.contentMode=UIViewContentModeScaleAspectFit;
    [myCodeBtn addTarget:self action:@selector(myCode) forControlEvents:UIControlEventTouchUpInside];
    [bottomBar addSubview:myCodeBtn];*/
}
- (void)setupScanWindowView
{
    //NSLog(@"scan windows rect %f,%f,%f,%f",self.ScanFieldRect.origin.x,self.ScanFieldRect.origin.y,self.ScanFieldRect.size.width,self.ScanFieldRect.size.height);
    _scanWindow = [[UIView alloc] initWithFrame:self.ScanFieldRect];
    _scanWindow.clipsToBounds = YES;
    [self.view addSubview:_scanWindow];
    CGFloat cornerImageSize=18;
    _scanNetImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"scan_net"]];
    UIButton *topLeft = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, cornerImageSize, cornerImageSize)];
    [topLeft setImage:[UIImage imageNamed:@"scan_1"] forState:UIControlStateNormal];
    [_scanWindow addSubview:topLeft];
    
    UIButton *topRight = [[UIButton alloc] initWithFrame:CGRectMake(self.ScanFieldRect.size.width-cornerImageSize, 0,cornerImageSize, cornerImageSize)];
    [topRight setImage:[UIImage imageNamed:@"scan_2"] forState:UIControlStateNormal];
    [_scanWindow addSubview:topRight];
    
    UIButton *bottomLeft = [[UIButton alloc] initWithFrame:CGRectMake(0, self.ScanFieldRect.size.height - cornerImageSize, cornerImageSize, cornerImageSize)];
    [bottomLeft setImage:[UIImage imageNamed:@"scan_3"] forState:UIControlStateNormal];
    [_scanWindow addSubview:bottomLeft];
    
    UIButton *bottomRight = [[UIButton alloc] initWithFrame:CGRectMake(self.ScanFieldRect.size.width - cornerImageSize, self.ScanFieldRect.size.height-cornerImageSize,  cornerImageSize, cornerImageSize)];
    [bottomRight setImage:[UIImage imageNamed:@"scan_4"] forState:UIControlStateNormal];
    [_scanWindow addSubview:bottomRight];
}

- (void)beginScanning
{
    //获取摄像设备
    AVCaptureDevice * device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    //创建输入流
    AVCaptureDeviceInput * input = [AVCaptureDeviceInput deviceInputWithDevice:device error:nil];
    if (!input) return;
    //创建输出流
    AVCaptureMetadataOutput * output = [[AVCaptureMetadataOutput alloc]init];
    //设置代理 在主线程里刷新
    [output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
    //设置有效扫描区域
    CGRect scanCrop=[self getScanCrop];
    [output setRectOfInterest:scanCrop];
    //NSLog(@"scan crop %f,%f,%f,%f",scanCrop.origin.x,scanCrop.origin.y,scanCrop.size.width,scanCrop.size.height);
    //初始化链接对象
    _session = [[AVCaptureSession alloc]init];
    //高质量采集率
    [_session setSessionPreset:AVCaptureSessionPresetHigh];
    
    [_session addInput:input];
    [_session addOutput:output];
    //设置扫码支持的编码格式(如下设置条形码和二维码兼容)
    output.metadataObjectTypes=@[AVMetadataObjectTypeQRCode,AVMetadataObjectTypeEAN13Code, AVMetadataObjectTypeEAN8Code, AVMetadataObjectTypeCode128Code];
    
    AVCaptureVideoPreviewLayer * layer = [AVCaptureVideoPreviewLayer layerWithSession:_session];
    layer.videoGravity=AVLayerVideoGravityResizeAspectFill;
    layer.frame=self.view.layer.bounds;
    //NSLog(@"layer frame %f,%f,%f,%f",layer.frame.origin.x,layer.frame.origin.y,layer.frame.size.width,layer.frame.size.height);
    if(CurrentOrientationMask==UIInterfaceOrientationMaskLandscapeRight){
        [layer connection].videoOrientation = AVCaptureVideoOrientationLandscapeRight;
    }
    [self.view.layer insertSublayer:layer atIndex:0];
    //开始捕获
    [_session startRunning];
}

-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
    if (metadataObjects.count>0) {
        [_session stopRunning];
        AVMetadataMachineReadableCodeObject * metadataObject = [metadataObjects objectAtIndex : 0 ];
        int content_len=(int)[metadataObject.stringValue lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
        [self release];
        [self.navigationController popViewControllerAnimated:YES];
    }
}
#pragma mark-> 我的相册
-(void)myAlbum{
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]){
        //1.初始化相册拾取器
        UIImagePickerController *controller = [[UIImagePickerController alloc] init];
        //2.设置代理
        controller.delegate = self;
        //3.设置资源：
        /**
         UIImagePickerControllerSourceTypePhotoLibrary,相册
         UIImagePickerControllerSourceTypeCamera,相机
         UIImagePickerControllerSourceTypeSavedPhotosAlbum,照片库
         */
        controller.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
        //4.随便给他一个转场动画
        controller.modalTransitionStyle=UIModalTransitionStyleFlipHorizontal;
        [self presentViewController:controller animated:YES completion:NULL];
    }else{
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"设备不支持访问相册，请在设置->隐私->照片中进行设置！" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alert show];
    }
    
}
#pragma mark-> imagePickerController delegate
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    //1.获取选择的图片
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    //2.初始化一个监测器
    CIDetector*detector = [CIDetector detectorOfType:CIDetectorTypeQRCode context:nil options:@{ CIDetectorAccuracy : CIDetectorAccuracyHigh }];
    
    [picker dismissViewControllerAnimated:YES completion:^{
        //监测到的结果数组
        NSArray *features = [detector featuresInImage:[CIImage imageWithCGImage:image.CGImage]];
        if (features.count >=1) {
            /**结果对象 */
            CIQRCodeFeature *feature = [features objectAtIndex:0];
            NSString *scannedResult = feature.messageString;
            UIAlertView * alertView = [[UIAlertView alloc]initWithTitle:@"扫描结果" message:scannedResult delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
          
        }
        else{
            UIAlertView * alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"该图片没有包含一个二维码！" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
        }
    }];
}
#pragma mark-> 闪光灯
-(void)openFlash:(UIButton*)button{
    button.selected = !button.selected;
    if (button.selected) {
        [self turnTorchOn:YES];
    }
    else{
        [self turnTorchOn:NO];
    }
    
}
#pragma mark-> 我的二维码
-(void)myCode{
}
#pragma mark-> 开关闪光灯
- (void)turnTorchOn:(BOOL)on{
    Class captureDeviceClass = NSClassFromString(@"AVCaptureDevice");
    if (captureDeviceClass != nil) {
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        if ([device hasTorch] && [device hasFlash]){
            [device lockForConfiguration:nil];
            if (on) {
                [device setTorchMode:AVCaptureTorchModeOn];
                [device setFlashMode:AVCaptureFlashModeOn];
                
            } else {
                [device setTorchMode:AVCaptureTorchModeOff];
                [device setFlashMode:AVCaptureFlashModeOff];
            }
            [device unlockForConfiguration];
        }
    }
}
#pragma mark 恢复动画
- (void)resumeAnimation
{
    CAAnimation *anim = [_scanNetImageView.layer animationForKey:@"translationAnimation"];
    if(anim){
        // 1. 将动画的时间偏移量作为暂停时的时间点
        CFTimeInterval pauseTime = _scanNetImageView.layer.timeOffset;
        // 2. 根据媒体时间计算出准确的启动动画时间，对之前暂停动画的时间进行修正
        CFTimeInterval beginTime = CACurrentMediaTime() - pauseTime;
        
        // 3. 要把偏移时间清零
        [_scanNetImageView.layer setTimeOffset:0.0];
        // 4. 设置图层的开始动画时间
        [_scanNetImageView.layer setBeginTime:beginTime];
        
        [_scanNetImageView.layer setSpeed:1.0];
        
    }else{
        
        CGFloat scanNetImageViewH = 241;
        CGFloat scanWindowH = self.ScanFieldRect.size.height;
        CGFloat scanNetImageViewW = _scanWindow.frame.size.width;
    
        _scanNetImageView.frame = CGRectMake(0, -scanNetImageViewH, scanNetImageViewW, scanNetImageViewH);
        CABasicAnimation *scanNetAnimation = [CABasicAnimation animation];
        scanNetAnimation.keyPath = @"transform.translation.y";
        scanNetAnimation.byValue = @(scanWindowH);
        scanNetAnimation.duration = 1.0;
        scanNetAnimation.repeatCount = MAXFLOAT;
        [_scanNetImageView.layer addAnimation:scanNetAnimation forKey:@"translationAnimation"];
        [_scanWindow addSubview:_scanNetImageView];
    }
    
    

}
#pragma mark-> 获取扫描区域的比例关系
-(CGRect)getScanCrop{
    GLfloat screen_width=CGRectGetWidth(self.view.frame);
    GLfloat screen_height=CGRectGetHeight(self.view.frame);
    CGFloat x,y,width,height;
    x = self.ScanFieldRect.origin.x/screen_width;
    y = self.ScanFieldRect.origin.y/screen_height;
    width = CGRectGetWidth(self.ScanFieldRect)/screen_width;
    height = CGRectGetHeight(self.ScanFieldRect)/screen_height;
    if(UIInterfaceOrientationMaskLandscapeRight==CurrentOrientationMask){
        return CGRectMake(x, y, width, height);
    }
    return CGRectMake(y, x, height, width);
}
#pragma mark-> 返回
- (void)disMiss{
    [self release];
    [self.navigationController popViewControllerAnimated:YES];
}
#pragma mark - UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if (buttonIndex == 0) {
        [self disMiss];
    } else if (buttonIndex == 1) {
        [_session startRunning];
    }
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
