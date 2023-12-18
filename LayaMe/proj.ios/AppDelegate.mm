#import "AppDelegate.h"
#import "ViewController.h"
#import "conchRuntime.h"
#import "LayaApp.h"
#import "BDHub.h"
#import "AFNetworking.h"
#import "AFNetworking/AFHTTPSessionManager.h"
#import "conchConfig.h"
static NSInteger i = 0;
static TencentOAuth* _tencentOAuth;
static AppDelegate* _appDelegate = nil;
static bool _init = false;
@implementation AppDelegate
+(TencentOAuth*)TencentOAuth{
    return _tencentOAuth;
}
+(AppDelegate*)Instance{
    return _appDelegate;
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] ;
    ViewController* pViewController  = [[ViewController alloc] init];
    _window.rootViewController = pViewController;
    [_window makeKeyAndVisible];
    
    _launchView = [[LaunchView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [_window.rootViewController.view addSubview:_launchView.view];
    //[self checkNetwork];
    [self start];
    return YES;
    
}
- (void)checkNetwork {
    // 1.延迟执行某一段代码
    __weak typeof(self)weakSelf = self;
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:2.0 repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"zzz");
        AFHTTPSessionManager *session = [AFHTTPSessionManager manager];
        session.requestSerializer = [AFJSONRequestSerializer serializer];
        session.responseSerializer = [AFHTTPResponseSerializer serializer];
        session.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json",@"text/javascript",@"text[表情]ml",@"text/喷脸ain",nil];
           [session GET:@"www.baidu.com" parameters: nil headers: nil progress:^(NSProgress * _Nonnull downloadProgress) {
               NSLog(@"下载的进度");
             } success:^(NSURLSessionDataTask * _Nonnull task, id _Nullable responseObject) {
               NSLog(@"请求成功:%@", responseObject);
                 [weakSelf start];
                 [timer invalidate];
             } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
               NSLog(@"请求失败:%@", error);
                 i++;
                 if (i > 20) {
                     [timer invalidate];
                 }
             }];
    }];
}
- (void)start
{
    if (!_init)
    {
        [WXApi registerApp:@"wx509326755a35d40a"  universalLink:@"https://app.layabox.com/layame/"];
        //_tencentOAuth = [[TencentOAuth alloc] initWithAppId:@"101799498" andDelegate:self];
        if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"mqq://"]]) {
            NSLog(@"可以启动URL");
        } else {
            NSLog(@"不可以启动URL");
        }
        [[BDHub alloc]initWithAppId:@"3"];
        [TencentOAuth setIsUserAgreedAuthorization:YES];
        _tencentOAuth = [[TencentOAuth alloc] initWithAppId:@"101799498" enableUniveralLink:YES universalLink:nil delegate:self];
        //_tencentOAuth = [[TencentOAuth alloc] initWithAppId:@"101799498" andDelegate: AppDelegate.Instance];
        _init = true;
    }
}
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    //return [conchConfig GetInstance]->m_nOrientationType;
    NSUInteger supportedInterfaceOrientations = UIInterfaceOrientationMaskPortrait| UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight | UIInterfaceOrientationMaskPortraitUpsideDown;

        return supportedInterfaceOrientations;
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
    m_kBackgroundTask = [application beginBackgroundTaskWithExpirationHandler:^{
        if(m_kBackgroundTask != UIBackgroundTaskInvalid )
        {
            NSLog(@">>>>>backgroundTask end");
            [application endBackgroundTask:m_kBackgroundTask];
            m_kBackgroundTask = UIBackgroundTaskInvalid;
        }
    }];
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

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url{
    NSLog(@"handleOpenURL:%@", url.absoluteString);
    NSString* sUrl = [url absoluteString];
    if ([sUrl hasPrefix:@"weixin"] || [sUrl hasPrefix:@"wx"]){
        return [WXApi handleOpenURL:url delegate:self];
    }
    else if ([sUrl hasPrefix:@"tencent"]) {
        return [TencentOAuth HandleOpenURL:url];
    }
    return NO;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation{
    NSLog(@"openURL:%@", url.absoluteString);
    NSString* sUrl = [url absoluteString];
    if ([sUrl hasPrefix:@"weixin"] || [sUrl hasPrefix:@"wx"]){
        return [WXApi handleOpenURL:url delegate:self];
    }
    else if ([sUrl hasPrefix:@"tencent"]) {
        return [TencentOAuth HandleOpenURL:url];
    }
    return NO;
}
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler{
    
    if ([userActivity.activityType isEqualToString:NSUserActivityTypeBrowsingWeb]){
        NSURL* url = userActivity.webpageURL;
        if (url){
            NSLog(@"universal link:%@", url);
        }
    }
    
    return [WXApi handleOpenUniversalLink:userActivity delegate:self];
}
- (void)onReq:(id)req{ //是微信终端向第三方程序发起请求，要求第三方程序响应。第三方程序响应完后必须调用sendRsp返回。在调用sendRsp返回时，会切回到微信终端程序界面。
}

- (void)onResp:(id)resp{
    //如果第三方程序向微信发送了sendReq的请求，那么onResp会被回调。sendReq请求调用后，会切到微信终端程序界面。
    //NSLog(@"LayaApp WX Resp:errCode=%d, errStr=%@", resp.errCode, resp.errStr);
    NSLog(@"LayaAPP RESP");
    if ([resp isKindOfClass:[SendAuthResp class]]){
        SendAuthResp* authResp = (SendAuthResp*)resp;
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:authResp.errCode], @"errCode",
                              authResp.code, @"code",
                              authResp.country, @"country",
                              authResp.errStr, @"errStr",
                              authResp.lang, @"lang",
                              authResp.state, @"state"
                              , nil];
        [LayaApp callback:@"login:" data:dict];
    }else if ([resp isKindOfClass:[SendMessageToWXResp class]]){
        SendMessageToWXResp* rsp = (SendMessageToWXResp*)resp;
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:rsp.errCode], @"errCode",
                              rsp.errStr, @"errStr"
                              , nil];
        [LayaApp callback:@"share:" data:dict];
    }
}
- (void)tencentDidLogin {
    NSLog(@"tencentDidLogin");
    if (_tencentOAuth.accessToken && 0 != [_tencentOAuth.accessToken length]){
        [_tencentOAuth getUserInfo];
    }else{
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:-1], @"errCode"
                              , nil];
        [LayaApp callback:@"login:" data:dict];
    }
    
}

-(void)getUserInfoResponse:(APIResponse *)response{
    NSString* nickname = @"";
    NSString* avatar = @"";
    NSString* gender = @"";
    if (URLREQUEST_SUCCEED == response.retCode && kOpenSDKErrorSuccess == response.detailRetCode){
        nickname = response.jsonResponse[@"nickname"];
        avatar = response.jsonResponse[@"figureurl_qq"];
        gender = response.jsonResponse[@"gender"];
//        for (id key in response.jsonResponse){
//            NSLog(@"key=%@,value=%@", key, response.jsonResponse[key]);
//        }
    }
    
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:0], @"errCode",
                          _tencentOAuth.accessToken, @"code",
                          _tencentOAuth.openId, @"openId",
                          nickname, @"nickname",
                          avatar, @"avatar",
                          gender, @"gender",
                          [NSNumber numberWithInt:[_tencentOAuth.expirationDate timeIntervalSince1970]], @"expirationDate"
                          , nil];
    [LayaApp callback:@"login:" data:dict];
}

- (void)tencentDidNotLogin:(BOOL)cancelled {
    NSLog(@"tencentDidNotLogin");
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:cancelled?-2:-3], @"errCode"
                          , nil];
    [LayaApp callback:@"login:" data:dict];
}

- (void)tencentDidNotNetWork {
    NSLog(@"tencentDidNotNetWork");
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:-4], @"errCode"
                          , nil];
    [LayaApp callback:@"login:" data:dict];
}
- (void)tencentDidLogout {
    [LayaApp callback:@"logout:" data: [[NSDictionary alloc] init]];
    //_tencentOAuth = [[TencentOAuth alloc] initWithAppId:@"101799498" andDelegate: AppDelegate.Instance];
    _tencentOAuth = [[TencentOAuth alloc] initWithAppId:@"101799498" enableUniveralLink:YES universalLink:nil delegate:self];
}
- (BOOL)tencentNeedPerformIncrAuth:(TencentOAuth *)tencentOAuth withPermissions:(NSArray *)permissions
{
    return YES;
}


- (BOOL)tencentNeedPerformReAuth:(TencentOAuth *)tencentOAuth
{
    return YES;
}
@end
