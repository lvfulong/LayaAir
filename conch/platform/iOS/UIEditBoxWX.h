/**
 @file			UIEditBox.h
 @brief         editBox类 继承 uitextField
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */
#import "UIKit/UIKit.h"

@interface UIEditBoxWX  : NSObject<UITextViewDelegate, UITextFieldDelegate>

@property (nonatomic, copy) NSString *defaultText;
@property (nonatomic, assign) int maxLength;
@property (nonatomic, assign) BOOL confirmHold;
@property (nonatomic, copy) NSString *confirmType;
@property (nonatomic, copy) NSString *prompt;
@property (nonatomic, copy) NSString *promptColor;
@property (nonatomic, copy) NSString *inputType;

- (instancetype)initWithMultiple:(BOOL)multiple;
//- (void)textViewDidChange:(UITextView *)textView;
-(void)hide;
-(void)clean;
- (void)becomeFirstResponder;

@end
