//
//  AppDelegate.h
//  MomoYo
//
//  Created by 范祎楠 on 2022/8/24.
//

#import <UIKit/UIKit.h>
#import "LaunchView.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    UIBackgroundTaskIdentifier m_kBackgroundTask;
}
@property (strong, nonatomic) LaunchView *launchView;
@property (strong, nonatomic) UIWindow *window;

@end

