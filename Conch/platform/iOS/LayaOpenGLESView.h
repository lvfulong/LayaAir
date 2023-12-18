#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface LayaOpenGLESView : UIView

@property(nonatomic, strong) EAGLContext *context;
- (instancetype)initWithFrame:(CGRect)frame contentScaleFactor:(CGFloat)contentScaleFactor;
@end
