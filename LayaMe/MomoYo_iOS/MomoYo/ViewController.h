#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <conchRuntime.h>

@interface ViewController : UIViewController
{
@public
    
    UIView*                    m_pView;
    
    EAGLContext*                m_pGLContext;
    
    conchRuntime*               m_pConchRuntime;
}
+(ViewController*)GetIOSViewController;
- (instancetype)initWithFrame:(CGRect)frame;
@end

