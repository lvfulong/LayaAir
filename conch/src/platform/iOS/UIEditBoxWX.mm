#import "UIEditBoxWX.h"
#import <Bindings/JSDevice.h>

#define TEXT_VIEW_HEIGHT 40
#define TEXT_VIEW_LEFT ([UIScreen mainScreen].bounds.size.width > [UIScreen mainScreen].bounds.size.height ? 40 : 8)
#define TEXT_VIEW_RIGHT (TEXT_VIEW_LEFT)
#define TEXT_VIEW_TOP ((BACKGROUND_VIEW_HEIGHT - TEXT_VIEW_HEIGHT) * 0.5)
#define TEXT_VIEW_MAX_LINES 3
#define TEXT_VIEW_MAX_HEIGHTS (TEXT_VIEW_MAX_LINES * TEXT_VIEW_HEIGHT)
#define BACKGROUND_VIEW_HEIGHT 58
#define SCREEN_WIDTH [UIScreen mainScreen].bounds.size.width
#define SCREEN_HEIGHT [UIScreen mainScreen].bounds.size.height
#define TEXT_COLOR [UIColor blackColor]

@interface UIEditBoxWX()
@property(nonatomic,strong) UIView *inputBackgroundView;
@property(nonatomic,assign) CGFloat keyboardHeight;
@property(nonatomic,strong) UITextField* textField;
@property(nonatomic,strong) UITextView* textView;
@property(nonatomic,strong) UIView* backgroundView;
@property(nonatomic,strong) UILabel *placeHolderLabel;
@property(nonatomic,assign) BOOL multiple;
@end

@implementation UIEditBoxWX

- (instancetype)initWithMultiple:(BOOL)multiple
{
    self = [super init];
    if (self) {
        _multiple = multiple;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
        
        [self initView];
    }
    return self;
}

- (void)dealloc
{
    if (_backgroundView != nil)
    {
        [_backgroundView removeFromSuperview];
        [self resignFirstResponder];
        _backgroundView = nil;
    }
    _textView = nil;
    _textField = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (void)setDefaultText:(NSString *)defaultText
{
    _defaultText = defaultText;
    if (_multiple) {
        _textView.text = self.defaultText;
    } else {
        _textField.text = self.defaultText;
    }
    
    if (![UIEditBoxWX isBlankString:_defaultText]) {
        _placeHolderLabel.hidden = true;
    }
    else {
        _placeHolderLabel.hidden = false;
    }
}

- (void)setConfirmType:(NSString *)confirmType
{
    _confirmType = confirmType;
    if (_textField) {
        if ([self.confirmType isEqualToString:@"done"])
            _textField.returnKeyType = UIReturnKeyDone;
        else if ([self.confirmType isEqualToString:@"next"])
            _textField.returnKeyType = UIReturnKeyNext;
        else if ([self.confirmType isEqualToString:@"search"])
            _textField.returnKeyType = UIReturnKeySearch;
        else if ([self.confirmType isEqualToString:@"go"])
            _textField.returnKeyType = UIReturnKeyGo;
        else if ([self.confirmType isEqualToString:@"send"])
            _textField.returnKeyType = UIReturnKeySend;
    }
}

- (void)setPrompt:(NSString *)prompt
{
    _prompt = prompt;
    if (![UIEditBoxWX isBlankString:self.prompt])
    {
        _placeHolderLabel.text = prompt;
    }
    
    if (![UIEditBoxWX isBlankString:self.prompt])
    {
        _textField.placeholder = self.prompt;
    }
}

- (void)setPromptColor:(NSString *)promptColor
{
    _promptColor = promptColor;
    if (![UIEditBoxWX isBlankString: self.promptColor])
    {
        _placeHolderLabel.textColor = [UIEditBoxWX colorWithHexString: self.promptColor alpha:1.0F];
    }
    
    if (![UIEditBoxWX isBlankString: self.promptColor])
    {
        [_textField setValue:[UIEditBoxWX colorWithHexString: self.promptColor alpha:1.0F] forKeyPath:@"placeholderLabel.textColor"];
    }
}

- (void)setInputType:(NSString *)inputType
{
    _inputType = inputType;
    if ([self.inputType isEqualToString:@"password"])
    {
        _textField.secureTextEntry = TRUE;
        _textField.keyboardType = UIKeyboardTypeDefault;
    }
    else if ([self.inputType isEqualToString:@"email"])
    {
        _textField.secureTextEntry = FALSE;
        _textField.keyboardType = UIKeyboardTypeEmailAddress;
    }
    else if ([self.inputType isEqualToString:@"number"])
    {
        _textField.secureTextEntry = FALSE;
        _textField.keyboardType = UIKeyboardTypeNumberPad;
    }
    else if ([self.inputType isEqualToString:@"url"])
    {
        _textField.secureTextEntry = FALSE;
        _textField.keyboardType = UIKeyboardTypeURL;
    }
    else// if ([self.inputType isEqualToString:@"text"])
    {
        _textField.secureTextEntry = FALSE;
        _textField.keyboardType = UIKeyboardTypeDefault;
    }
}

-(void)initView
{
    if (_backgroundView != nil)
    {
        [_backgroundView removeFromSuperview];
        _backgroundView = nil;
    }
    _textView = nil;
    _textField = nil;
    
    
    _backgroundView = [[UIView alloc] init];
    //F7F7F7
    _backgroundView.backgroundColor = [UIColor colorWithRed: 247 / 255.0 green: 247 / 255.0 blue: 247 / 255.0 alpha: 1];
    _backgroundView.frame = CGRectMake(0, SCREEN_HEIGHT, SCREEN_WIDTH, BACKGROUND_VIEW_HEIGHT);
    
    if (_multiple)
    {
        [self createTextView];
        _textView.frame = CGRectMake(TEXT_VIEW_LEFT, TEXT_VIEW_TOP, SCREEN_WIDTH - TEXT_VIEW_LEFT - TEXT_VIEW_RIGHT, TEXT_VIEW_HEIGHT);
        [_backgroundView addSubview:_textView];
    }
    else
    {
        [self createTextField];
        _textField.frame = CGRectMake(TEXT_VIEW_LEFT, TEXT_VIEW_TOP, SCREEN_WIDTH - TEXT_VIEW_LEFT - TEXT_VIEW_RIGHT, TEXT_VIEW_HEIGHT);
        [_backgroundView addSubview:_textField];
        //[textField setValue:[UIFont boldSystemFontOfSize:16] forKeyPath:@"_placeholderLabel.font"];
    }
    [UIApplication.sharedApplication.delegate.window.rootViewController.view addSubview:_backgroundView];
}
- (void)becomeFirstResponder
{
    if (_multiple) {
        [_textView becomeFirstResponder];
    } else {
        [_textField becomeFirstResponder];
    }
}
- (void)resignFirstResponder
{
    if (_multiple) {
        [_textView resignFirstResponder];
    } else {
        [_textField resignFirstResponder];
    }
}

- (UITextField *)createTextField
{
    if (!_textField) {
        _textField = [[UITextField alloc] init];
        _textField.font = [UIFont systemFontOfSize: 16];
        UIView* paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 8, 20)];
        _textField.leftView = paddingView;
        _textField.leftViewMode = UITextFieldViewModeAlways;
        _textField.delegate = self;
        _textField.textColor = TEXT_COLOR;
        _textField.backgroundColor =  UIColor.whiteColor;
        _textField.layer.cornerRadius = 13;
        _textField.layer.masksToBounds = YES;
        _textField.keyboardAppearance = UIKeyboardAppearanceLight;
        [_textField addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    }
    return _textField;
}

- (UITextView *)createTextView
{
    if (!_textView) {
        _textView = [[UITextView alloc] init];
        _textView.font = [UIFont systemFontOfSize: 16];
        _textView.layer.cornerRadius = 13;
        _textView.layer.masksToBounds = YES;
        _textView.textColor = TEXT_COLOR;
        _textView.backgroundColor = UIColor.whiteColor;
        _textView.delegate = self;
        _textView.textContainerInset = UIEdgeInsetsMake(8, 8, 8, 8);
        _textView.keyboardAppearance = UIKeyboardAppearanceLight;
        UILabel *placeHolderLabel = [[UILabel alloc] init];
        _placeHolderLabel = placeHolderLabel;
        placeHolderLabel.numberOfLines = 0;
        [placeHolderLabel sizeToFit];
        placeHolderLabel.font = [UIFont systemFontOfSize:16];
        [_textView addSubview:placeHolderLabel];
        [_textView setValue:placeHolderLabel forKey:@"placeholderLabel"];
    }
    return _textView;
}

+ (BOOL)isBlankString:(NSString *)str {
    NSString *string = str;
    if (string == nil || string == NULL) {
        return YES;
    }
    if ([string isKindOfClass:[NSNull class]]) {
        return YES;
    }
    if ([[string stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length]==0) {
        return YES;
    }
    
    return NO;
}
+ (UIColor *)colorWithHexString:(NSString *)hexString alpha:(CGFloat)alpha {
    unsigned int red = 0, green = 0, blue = 0;
    [[NSScanner scannerWithString:[hexString substringWithRange:NSMakeRange(0+1, 2)]] scanHexInt:&red];
    [[NSScanner scannerWithString:[hexString substringWithRange:NSMakeRange(2+1, 2)]] scanHexInt:&green];
    [[NSScanner scannerWithString:[hexString substringWithRange:NSMakeRange(4+1, 2)]] scanHexInt:&blue];

    return [UIColor colorWithRed:((red)/255.0) green:((green)/255.0) blue:((blue)/255.0) alpha:(alpha)];
}
-(void)handleKeyboardConfirm
{
    NSString* text = nil;
    if (_multiple)
    {
        text = _textView.text;
        text = [text stringByReplacingOccurrencesOfString:@"\u2006" withString:@""];
    }
    else
    {
        text = _textField.text;
        text = [text stringByReplacingOccurrencesOfString:@"\u2006" withString:@""];
    }
    laya::JSDevice::handleKeyboardConfirm(text.UTF8String);
    laya::JSDevice::handleKeyboardComplete(text.UTF8String);
}
-(void)hideKeyboard
{
    //if (!self.confirmHold)
    {
        [self handleKeyboardConfirm];
        [_backgroundView removeFromSuperview];
        [self resignFirstResponder];
        _backgroundView = nil;

        _textView = nil;
        _textField = nil;
    }
}


- (void)keyboardWillShow:(NSNotification*) notification
{
    CGRect frame = [notification.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    double duration = [notification.userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    [self inputViewAnimationWith:duration frame:frame];
    
}

- (void)keyboardWillHide:(NSNotification*) notification
{
    CGRect frame = [notification.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    double duration = [notification.userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    [self inputViewAnimationWith:duration frame:frame];
}

-(void)inputViewAnimationWith:(double)duration frame:(CGRect)frame
{
    CGRect currentFrame = _backgroundView.frame;
    
    [UIView animateWithDuration:duration animations:^{
        CGRect resultFrame;
        
        if (frame.origin.y == SCREEN_HEIGHT)
        {
            resultFrame=CGRectMake(currentFrame.origin.x, SCREEN_HEIGHT-currentFrame.size.height, SCREEN_WIDTH, currentFrame.size.height);
            self.keyboardHeight = 0;
        }
        else
        {
            resultFrame=CGRectMake(currentFrame.origin.x,SCREEN_HEIGHT-currentFrame.size.height-frame.size.height , SCREEN_WIDTH, currentFrame.size.height);
            self.keyboardHeight = frame.size.height;
        }
        
        self->_backgroundView.frame = resultFrame;
   }];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    
    [self hideKeyboard];
    return YES;
}
- (void)textFieldDidChange:(UITextField *)textField
{
    if (textField.markedTextRange != nil)
        return;

    if (textField.text.length > self.maxLength)
    {
        NSRange rangeIndex = [textField.text rangeOfComposedCharacterSequenceAtIndex:self.maxLength];
        textField.text = [textField.text substringToIndex:rangeIndex.location];
    }
    NSString* text = textField.text;
    text = [text stringByReplacingOccurrencesOfString:@"\u2006" withString:@""];
    laya::JSDevice::handleKeyboardInput([text UTF8String]);
}
-(void)textViewDidChange:(UITextView *)textView
{
    
    NSString* str = textView.text;
    CGSize maxSize = CGSizeMake(textView.bounds.size.width, MAXFLOAT);
    CGRect frame = [str boundingRectWithSize:maxSize options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading attributes:@{NSFontAttributeName:textView.font} context:nil];
    CGFloat newTextViewHeight = TEXT_VIEW_HEIGHT;
    if (frame.size.height + 10 > TEXT_VIEW_HEIGHT)
    {
        newTextViewHeight = frame.size.height + 10;
    }

    if (newTextViewHeight > TEXT_VIEW_MAX_HEIGHTS)
    {
        newTextViewHeight = TEXT_VIEW_MAX_HEIGHTS;
    }
    
    CGFloat width = SCREEN_WIDTH;
    CGFloat newBackgroundViewHeight = newTextViewHeight + 2 * TEXT_VIEW_TOP;
    _backgroundView.frame = CGRectMake(0, (SCREEN_HEIGHT - self.keyboardHeight) - newBackgroundViewHeight, width, newBackgroundViewHeight);
    
    if (_multiple)
    {
        _textView.frame = CGRectMake(TEXT_VIEW_LEFT, TEXT_VIEW_TOP , width - TEXT_VIEW_LEFT - TEXT_VIEW_RIGHT, newTextViewHeight);
        
        if (_textView.text.length > self.maxLength)
            _textView.text = [_textView.text substringToIndex:self.maxLength];
        
        NSString* text = _textView.text;
        text = [text stringByReplacingOccurrencesOfString:@"\u2006" withString:@""];
        laya::JSDevice::handleKeyboardInput([text UTF8String]);
    }
    else
    {
        
    }
}
@end
