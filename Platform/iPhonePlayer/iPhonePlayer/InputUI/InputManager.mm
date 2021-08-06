#import "InputManager.h"
@implementation InputManager
-(id)initWithParentView:(UIView *)parent{
    NSArray *itemArray = [[NSBundle mainBundle] loadNibNamed:@"InputView" owner:nil options:nil];
    for (id item in itemArray) {
        if ([item isMemberOfClass:[InputView class]]) {
            _inputView = item;
            _inputView.y = kScreenHeight - _inputView.viewHeight;
            _inputView.delegate = self;
            [_inputView retain];
            [_inputView setHidden:YES];
            [parent addSubview:_inputView];
        }
    }
    return self;
}
-(void)dealloc{
    [super dealloc];
}
-(void)inputView:(InputView *)inputView submitContent:(NSString *)message complete:(void (^)(BOOL))completeBlock{
    [_inputView.commentTV resignFirstResponder];
    [_inputView setHidden:YES];
    completeBlock(YES);
}
-(void)ShowKeyboard:(const char*)utf8Str{
    [_inputView AdjustInputView];
    [_inputView setHidden:NO];
    [_inputView SetText:utf8Str];
    [_inputView.commentTV performSelector:@selector(becomeFirstResponder) withObject:nil afterDelay:0.1];
}
@end
