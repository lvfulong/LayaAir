#import <UIKit/UIKit.h>
#import "LaunchView.h"

@class NavViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) LaunchView *launchView;
@property (strong, nonatomic) NavViewController* navigationController;
@end
