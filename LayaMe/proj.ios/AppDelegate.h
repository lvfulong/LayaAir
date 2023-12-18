#import <UIKit/UIKit.h>
#import "LaunchView.h"
#import "WXApi.h"
#import <TencentOpenAPI/TencentOAuth.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate, WXApiDelegate, TencentSessionDelegate>
{
@public
    UIBackgroundTaskIdentifier m_kBackgroundTask;
}
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) LaunchView *launchView;
+(TencentOAuth*)TencentOAuth;
+(AppDelegate*)Instance;
@end
