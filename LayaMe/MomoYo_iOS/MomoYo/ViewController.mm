#import "ViewController.h"
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>
#import <CoreTelephony/CTCellularData.h>
#import "Reachability/Reachability.h"

extern std::string g_ConfigJS;
extern NSString* url;
extern NSString* qudao;
extern NSInteger qudaoID;
extern NSString* channel;
extern long g_startMS;
long long g_nowMS = 0;

@implementation ViewController
{
    CTCellularData *_cellularData;
    LayaReachability *_pNetworkListener;
    CGRect _frame;
    CADisplayLink* _displayLink;
    bool _isInit;
}

static ViewController* g_pIOSMainViewController = nil;
//------------------------------------------------------------------------------
+(ViewController*)GetIOSViewController
{
    return g_pIOSMainViewController;
}
//------------------------------------------------------------------------------
- (instancetype)initWithFrame:(CGRect)frame {
    self = [super init];
    if( self != nil )
    {
        g_pIOSMainViewController = self;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:LayakReachabilityChangedNotification object:nil];
        _pNetworkListener = [LayaReachability reachabilityForInternetConnection];
        [_pNetworkListener startNotifier];
        _frame = frame;
        _isInit = false;
        return self;
    }
    return nil;
}
//------------------------------------------------------------------------------
- (void)viewDidLoad
{
    [super viewDidLoad];
    //保持屏幕常亮，可以通过脚本设置
    m_pConchRuntime = [[conchRuntime alloc] initWithFrame:_frame URL:url];
    self.view = m_pConchRuntime->m_pView;
    [self setupGlobalConfig];
    
    _cellularData = [[CTCellularData alloc] init];
    //选择仅无线网络时也返回kCTCellularDataRestricted，因此需要Reachability协助判断
    if (_cellularData.restrictedState == kCTCellularDataNotRestricted || _pNetworkListener.currentReachabilityStatus != NotReachable) {
        [self initConch];
    } else {
        __weak ViewController* weakSelf = self;
        [self networkAuthorizationAvalible:^{
            ViewController *strongSelf = weakSelf;
            dispatch_async(dispatch_get_main_queue(), ^{
                [strongSelf initConch];
            });
        }];
    }
}
- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    const CGSize size = self.view.bounds.size;
    [m_pConchRuntime onResize: size];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    if (@available(iOS 14, *)) {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            NSLog(@"App tracking authorization: %lu", (unsigned long)status);
        }];
    }
    //[self.navigationController setNavigationBarHidden:YES animated:NO];
}
//------------------------------------------------------------------------------
- (void)dealloc
{
    [self destory];
}
-(void)destory
{
    if (_displayLink != nil)
    {
        [_displayLink invalidate];
        _displayLink = nil;
        [m_pConchRuntime destroy];
        m_pConchRuntime = nil;
        [[NSNotificationCenter defaultCenter] removeObserver:self name:LayakReachabilityChangedNotification object:nil];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillEnterForegroundNotification object:nil];
    }
}
//------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    //conchRuntime 内存警告的时候的处理
    [m_pConchRuntime didReceiveMemoryWarning];
}
//------------------------------------------------------------------------------
-(void)update:(CADisplayLink*)displayLink
{
    //conchRuntime renderFrame
    [m_pConchRuntime update];
}
//-------------------------------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesBegan:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesMoved:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesEnded:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesCancelled:touches withEvent:event];
}
//-------------------------------------------------------------------------------
-(NSUInteger)supportedInterfaceOrientations
{
    /*
     UIInterfaceOrientationMaskPortrait,             ===2
     UIInterfaceOrientationMaskPortraitUpsideDown,   ===4
     UIInterfaceOrientationMaskLandscapeLeft,        ===8
     UIInterfaceOrientationMaskLandscapeRight,       ===16
     */
    return [conchConfig GetInstance]->m_nOrientationType;
}
//-------------------------------------------------------------------------------
- (BOOL)shouldAutorotate
{
    return YES;//支持转屏
}
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    [m_pConchRuntime onOrientationChanged:size];
    [m_pConchRuntime hideEditBoxWX];
}

- (void)initConch
{
    if (_isInit)
        return;
    _isInit = true;
    //[conchRuntime setAppLaunchStartTimeInMs:g_startMS];
    [m_pConchRuntime initConch];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init] ;

    [formatter setDateStyle:NSDateFormatterMediumStyle];

    [formatter setTimeStyle:NSDateFormatterShortStyle];

    [formatter setDateFormat:@"YYYY-MM-dd HH:mm:ss SSS"];
    NSTimeZone* timeZone = [NSTimeZone timeZoneWithName:@"Asia/Shanghai"];
    [formatter setTimeZone:timeZone];
    NSDate *datenow = [NSDate date];
    if (g_nowMS == 0)
    {
        g_nowMS = (long long)([datenow timeIntervalSince1970] * 1000);
    }
    //[conchRuntime setAppLaunchTimeInMs:(g_nowMS - g_startMS)];

    NSLog(@"initConch  %lld  %lld   %lld ", g_startMS, g_nowMS, (g_nowMS - g_startMS));
        
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    if ([_displayLink respondsToSelector: @selector(preferredFramesPerSecond)] == YES)
    {
        _displayLink.preferredFramesPerSecond = 60;
    }
    [_displayLink addToRunLoop: [NSRunLoop mainRunLoop] forMode: NSDefaultRunLoopMode];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidEnterBackground:) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillEnterForeground:) name:UIApplicationWillEnterForegroundNotification object:nil];
}

- (void)setupGlobalConfig
{
    g_ConfigJS += "window._qudao = \"" + std::string([qudao UTF8String]) + "\";";
    g_ConfigJS += "window._qudao_id = " + std::to_string(qudaoID) + ";";
    g_ConfigJS += "window._channel = \"" + std::string([channel UTF8String]) + "\";";
}

- (void)networkAuthorizationAvalible:(void(^)())changeAvaliable
{
    _cellularData.cellularDataRestrictionDidUpdateNotifier = ^(CTCellularDataRestrictedState state) {
        switch (state) {
            case kCTCellularDataRestricted:
                break;
            case kCTCellularDataNotRestricted:
                changeAvaliable();
                break;
                //未知，第一次请求
            case kCTCellularDataRestrictedStateUnknown:
                break;
            default:
                break;
        };
    };
}
- (void)networkStateChange
{
    LayaNetworkStatus networkStatus = _pNetworkListener.currentReachabilityStatus;
    if (networkStatus != NotReachable) {
        [self initConch];
    }
}

- (void)applicationDidEnterBackground:(NSNotification *)notification {
    _displayLink.paused = TRUE;
}

- (void)applicationWillEnterForeground:(NSNotification *)notification {
    _displayLink.paused = FALSE;
}
@end
