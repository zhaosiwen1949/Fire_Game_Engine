//
//  CommentView.h
//  GuDaShi
//
//  Created by LOLITA on 2017/9/4.
//  Copyright © 2017年 songzhaojie. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIView+AdjustFrame.h"
#import "MyMacro.h"
@protocol InputViewDelegate;
@interface InputView : UIView
@property (strong, nonatomic) IBOutlet UITextView *commentTV;
@property (strong, nonatomic) IBOutlet UILabel *tipLabel; // 提示
@property (strong, nonatomic) IBOutlet UIButton *senderBtn;
@property (nonatomic,strong) id <InputViewDelegate> delegate;
-(void)SetText:(const char*)utf8Str;
-(void)AdjustInputView;
@end
@protocol InputViewDelegate <NSObject>
@optional
-(void)inputView:(InputView*)inputView submitContent:(NSString*)message complete:(void(^)(BOOL success))completeBlock;
@end
