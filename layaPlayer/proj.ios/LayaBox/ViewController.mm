#import "ViewController.h"
#import <CoreTelephony/CTCellularData.h>
#import "Reachability/Reachability.h"
#import "UIFloatPanel.h"
@implementation ViewController
{
    CGRect _frame;
    CADisplayLink* _displayLink;
    conchRuntime* _conchRuntime;
    CTCellularData *_cellularData;
    LayaReachability *_pNetworkListener;
    bool _isInit;
    UIFloatPanel* m_floatPanel;
}
static ViewController* g_pIOSMainViewController = nil;
//------------------------------------------------------------------------------
+(ViewController*)GetIOSViewController
{
    return g_pIOSMainViewController;
}
-(void)showFloatPanel:(bool)show
{
    m_floatPanel.hidden = !show;
}
//------------------------------------------------------------------------------
-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super init];
    if( self != nil )
    {
        g_pIOSMainViewController = self;
        _frame = frame;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:LayakReachabilityChangedNotification object:nil];
        _pNetworkListener = [LayaReachability reachabilityForInternetConnection];
        [_pNetworkListener startNotifier];
        _isInit = false;
        return self;
    }
    return Nil;
}
//------------------------------------------------------------------------------
- (void)viewDidLoad
{
    [super viewDidLoad];
    //保持屏幕常亮，可以通过脚本设置
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    _conchRuntime = [[conchRuntime alloc] initWithFrame:_frame URL:nil];
    self.view = _conchRuntime->m_pView;
    m_floatPanel = [[UIFloatPanel alloc] initWithParentView: _conchRuntime->m_pView];
    m_floatPanel.hidden = true;
    _cellularData = [[CTCellularData alloc] init];
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
//------------------------------------------------------------------------------
- (void)dealloc
{
    [self destroy];
     m_floatPanel = nil;
}
//------------------------------------------------------------------------------
- (void)destroy
{
    if (_displayLink != nil)
    {
        [_displayLink invalidate];
        _displayLink = nil;
        [_conchRuntime destroy];
        _conchRuntime = nil;
        [[NSNotificationCenter defaultCenter] removeObserver:self name:LayakReachabilityChangedNotification object:nil];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillEnterForegroundNotification object:nil];
    }
}
//------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    [_conchRuntime didReceiveMemoryWarning];
}
//------------------------------------------------------------------------------
- (void)update:(CADisplayLink*)displayLink
{
    [_conchRuntime update];
}
//-------------------------------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_conchRuntime touchesBegan:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_conchRuntime touchesMoved:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_conchRuntime touchesEnded:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_conchRuntime touchesCancelled:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (NSUInteger)supportedInterfaceOrientations
{
    /*
     UIInterfaceOrientationMaskPortrait,             ===2
     UIInterfaceOrientationMaskPortraitUpsideDown,   ===4
     UIInterfaceOrientationMaskLandscapeLeft,        ===8
     UIInterfaceOrientationMaskLandscapeRight,       ===16
     */
     return [conchRuntime getOrientationMask];
}
//-------------------------------------------------------------------------------
- (BOOL)shouldAutorotate
{
    return YES;//支持转屏
}
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    [_conchRuntime onOrientationChanged: size];
}
- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    const CGSize size = self.view.bounds.size;
    [_conchRuntime onResize: size];
}
- (void)initConch
{
    if (_isInit)
        return;
    _isInit = true;
    [_conchRuntime initConch];
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    if ([_displayLink respondsToSelector: @selector(preferredFramesPerSecond)] == YES)
    {
        _displayLink.preferredFramesPerSecond = 60;
    }
    [_displayLink addToRunLoop: [NSRunLoop mainRunLoop] forMode: NSDefaultRunLoopMode];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidEnterBackground:) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillEnterForeground:) name:UIApplicationWillEnterForegroundNotification object:nil];
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
