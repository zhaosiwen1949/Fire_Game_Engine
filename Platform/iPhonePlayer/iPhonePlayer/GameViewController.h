//
//  GameViewController.h
//  iPhonePlayer
//
//  Created by Heck on 16/5/17.
//  Copyright © 2016年 battlefire. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <QBImagePicker/QBImagePicker.h>
@interface GameViewController : GLKViewController<QBImagePickerControllerDelegate>
-(void)OrientChanged:(NSNotification*)noti;
-(void)ShowKeyboard:(const char*)utf8Str;
@end
