#import "UIKit/UIKit.h"

@interface UIEditBoxWX: NSObject<UITextViewDelegate, UITextFieldDelegate>

@property (nonatomic, copy) NSString *defaultText;
@property (nonatomic, assign) int maxLength;
@property (nonatomic, assign) BOOL confirmHold;
@property (nonatomic, copy) NSString *confirmType;
@property (nonatomic, copy) NSString *prompt;
@property (nonatomic, copy) NSString *promptColor;
@property (nonatomic, copy) NSString *inputType;

-(instancetype)initWithMultiple: (BOOL)multiple;
-(void)hideKeyboard;
-(void)becomeFirstResponder;

@end
