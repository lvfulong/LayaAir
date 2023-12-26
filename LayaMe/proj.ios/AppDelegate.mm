#import "AppDelegate.h"
#import "ViewController.h"
#import "conchRuntime.h"
#import "conchConfig.h"
#import <UserNotifications/UserNotifications.h>


extern NSUInteger GCEnvironment;
extern std::string LAYA_NATIVE_FILE_CACHE_TMP_PATH;
@interface AppDelegate () <UNUserNotificationCenterDelegate>
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self registerForRemoteNotifications:application];
    _window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] ;
    ViewController* pViewController  = [[ViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    _window.rootViewController = pViewController;
    [_window makeKeyAndVisible];
    
    _launchView = [[LaunchView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [_window.rootViewController.view addSubview:_launchView.view];
    
    UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
    [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert|UNAuthorizationOptionSound|UNAuthorizationOptionBadge) completionHandler:^(BOOL granted, NSError * _Nullable error) {
    }];
    //NSString* tempPath = [[NSString alloc] initWithUTF8String:LAYA_NATIVE_FILE_CACHE_TMP_PATH.c_str()];
    NSURLCache *URLCache = [[NSURLCache alloc] initWithMemoryCapacity:4 * 1024 * 1024
                                                           diskCapacity:100 * 1024 * 1024
                                                               diskPath:nil];
    [NSURLCache setSharedURLCache:URLCache];
    
    // Override point for customization after application launch.
    return YES;
}

//注册远程通知获取token
-(void)registerForRemoteNotifications:(UIApplication *)application {
     UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
    
     center.delegate = self;
     [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert | UNAuthorizationOptionBadge | UNAuthorizationOptionSound) completionHandler:^(BOOL granted, NSError * _Nullable error) {
         if (granted) {
             // 点击允许
             NSLog(@"注册成功");
         }
         [center getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings * _Nonnull settings) {
             NSLog(@"%@", settings);
        }];
     }];
    [application registerForRemoteNotifications];
}

#pragma mark - Private

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return [conchConfig GetInstance]->m_nOrientationType;
}
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    //m_kBackgroundTask = [application beginBackgroundTaskWithExpirationHandler:^{
    //    if(self->m_kBackgroundTask != UIBackgroundTaskInvalid )
    //    {
    //        NSLog(@">>>>>backgroundTask end");
    //        [application endBackgroundTask:self->m_kBackgroundTask];
    //        self->m_kBackgroundTask = UIBackgroundTaskInvalid;
    //    }
    //}];
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions
{
    NSLog(@"application:willFinishLaunchingWithOptions:");
}
@end
