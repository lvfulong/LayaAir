#import "conchRuntime.h"
#import "Reachability/Reachability.h"
#import "Audio/JCMp3Player.h"
#import <utils/JCColor.h>
#import "JCScriptRuntime.h"
#import "CToObjectC.h"
#import "LayaToast.h"
#import "Notification/LayaNotifyManager.h"
#import <downloadCache/JCIosFileSource.h>
#import "JCConch.h"
#import "Audio/JCAudioManager.h"
#import "LayaEditBoxDelegate.h"
#import "LayaEditBox.h"
#import "TouchFilter.h"
#import <binder/JSBind.h>
//#import <Bindings/JSLayaNative.h>
#import "LayaAlert.h"
#import "CToObjectCIOS.h"
#import "Reflection/refection.h"
#import <AudioToolBox/AudioSession.h>
#import "LayaDeviceSensor.h"
#import <resource/JCFileResManager.h>
#import "JCSystemConfig.h"
#import "UIEditBoxWX.h"
#import "LayaVideoPlayer.h"
#import <Bindings/JSConchConfig.h>
#import "LayaOpenGLESView.h"


@implementation FuncObj
-(id)init:(std::function<void(void)>)func
{
    self = [super init];
    if( self != nil)
    {
        m_Fuction = func;
        return self;
    }
    return nil;
}
@end

@implementation conchRuntime
{
    laya::BackendOptions m_options;
}

extern bool g_bGLCanvasSizeChanged;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern std::string gRedistPath;
extern std::string gResourcePath;
extern std::string gAssetRootPath;
extern bool gbBackground;
//------------------------------------------------------------------------------
static conchRuntime* g_pIOSConchRuntime = nil;

+(conchRuntime*)GetIOSConchRuntime
{
    return g_pIOSConchRuntime;
}
-(id)initWithFrame:(CGRect)frame URL:(NSString*)pUrl
{
    self = [super init];
    if( self != nil )
    {
        if (pUrl != nil)
        {
            laya::g_kSystemConfig.m_strStartURL = pUrl.UTF8String;
        }
        m_bEngineInited = false;
        m_pView = nil;
        m_pTouchFilter = NULL;
        for( int i = 0; i < 10; i++ )
        {
            m_vTouch[i] = NULL;
        }
        m_nTouchCount = 0;
        m_nGLViewOffset = 0;
        m_pEditBox = NULL;
        m_pEditBoxDelegate = NULL;
        m_pMp3Player = NULL;
        m_pNetworkListener = NULL;
        m_fRetinaValue = 1;
        m_nsRootResourcePath = nil;
        m_nsRootCachePath = nil;
        m_pNSTimer = nil;
        m_bIgnoreCurEvent = false;
        m_fIOSVersion = 0;
        g_pIOSConchRuntime = self;
        m_pJSRunLoop = nil;
        m_pLayaAlert=[[LayaAlert alloc]init];
        m_pReflection = [[Reflection alloc] init];
        [self setConchRuntimeParam:frame];
        m_nCurrentOrientation=-1;
        m_bStopEngine=false;
        return self;
    }
    return nil;
}

-(void)setConchRuntimeParam:(CGRect)frame;
{
    
    laya::g_kSystemConfig.m_nOrientationType =
    [self parseInterfaceOrientations: [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations"]];
    
    
    
    float fIOSVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    //获得retina屏的缩放值
    float fRetinaValue = [UIScreen mainScreen].scale;
    float fIosVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    if( fIosVersion >= 8.0 )
        fRetinaValue = [UIScreen mainScreen].nativeScale;
    
    
    m_fRetinaValue = fRetinaValue;
    
    m_pView = [[LayaOpenGLESView alloc] initWithFrame:frame contentScaleFactor:m_fRetinaValue];
    m_options.nativeLayer = (__bridge void*)(CAEAGLLayer*)m_pView.layer;
    m_options.nativeContext = (__bridge void*)((LayaOpenGLESView*)m_pView).context;
    
    NSString *colorFormat;
    if (m_options.alpha) {
        colorFormat = kEAGLColorFormatRGBA8;
    }
    else {
        colorFormat = kEAGLColorFormatRGB565;
    }
    ((CAEAGLLayer*)m_pView.layer).opaque = YES;
    ((CAEAGLLayer*)m_pView.layer).drawableProperties = @{ kEAGLDrawablePropertyRetainedBacking : m_options.preserveBackBuffer ? @(TRUE) : @(FALSE),
                                                          kEAGLDrawablePropertyColorFormat : colorFormat };
    
    
    m_nsRootResourcePath = [self getResourcePath];
    m_nsRootCachePath = [self getRootCachePath];
    //NSLog(@"AppVersion=%@",[conchConfig GetInstance]->m_sAppVersion );
    //if( [conchConfig GetInstance]->m_bNotification)
    //{
    //    [[LayaNotifyManager GetInstance]deleteAllNotify];
    //}
    m_pTouchFilter = new JCTouchFilter();
    m_nTouchCount = 0;
    m_fIOSVersion = fIosVersion;
    
    m_pResolution = new CGPoint();
    
    CGRect kRect = [UIScreen mainScreen].bounds;
    int nOrientationType = laya::g_kSystemConfig.m_nOrientationType;
    
    if( ( nOrientationType & UIInterfaceOrientationMaskLandscapeLeft ) == UIInterfaceOrientationMaskLandscapeLeft ||
       ( nOrientationType & UIInterfaceOrientationMaskLandscapeRight ) ==  UIInterfaceOrientationMaskLandscapeRight )
    {
        m_pResolution->x = MAX( kRect.size.width , kRect.size.height );
        m_pResolution->y = MIN( kRect.size.width , kRect.size.height );
    }
    //竖屏
    else if( ( nOrientationType & UIInterfaceOrientationMaskPortrait ) == UIInterfaceOrientationMaskPortrait ||
            ( nOrientationType & UIInterfaceOrientationMaskPortraitUpsideDown ) ==  UIInterfaceOrientationMaskPortraitUpsideDown )
    {
        m_pResolution->x = MIN( kRect.size.width , kRect.size.height );
        m_pResolution->y = MAX( kRect.size.width , kRect.size.height );
    }
    [self initNetworkListener];
    int nNetworkState = [self checkNetworkState];
    //[self onGLReady:m_pResolution->x*m_fRetinaValue  height:m_pResolution->y*m_fRetinaValue options:options];
    m_nGLViewOffset = 0;
    m_pView.multipleTouchEnabled = true;
    m_pEditBoxDelegate = [[LayaEditBoxDelegate alloc]init];

    [m_pEditBoxDelegate setRetinaValue:m_fRetinaValue];
    m_pEditBox = [[LayaEditBox alloc]initWithParentView:m_pView EditBoxDelegate:m_pEditBoxDelegate ScreenRatio:m_fRetinaValue ];
    m_pMp3Player = [[JCMp3Player alloc] init];
}

void AudioEngineInterruptionListenerCallback(void* user_data, UInt32 interruption_state)
{
    ALCcontext *context = laya::JCAudioManager::GetInstance()->m_pWavPlayer->m_pContext;
    if (kAudioSessionBeginInterruption == interruption_state)
    {
        alcMakeContextCurrent(nullptr);
    }
    else if (kAudioSessionEndInterruption == interruption_state)
    {
        OSStatus result = AudioSessionSetActive(true);
        if (result) NSLog(@"Error setting audio session active! %d\n", result);
        
        alcMakeContextCurrent(context);
    }
}

-(void)handleInterruption:(NSNotification*)notification
{
    static bool resumeOnBecomingActive = false;
    ALCcontext *context = laya::JCAudioManager::GetInstance()->m_pWavPlayer->m_pContext;
    if ([notification.name isEqualToString:AVAudioSessionInterruptionNotification]) {
        NSInteger reason = [[[notification userInfo] objectForKey:AVAudioSessionInterruptionTypeKey] integerValue];
        if (reason == AVAudioSessionInterruptionTypeBegan) {
            alcMakeContextCurrent(NULL);
        }
        
        if (reason == AVAudioSessionInterruptionTypeEnded) {
            if ([UIApplication sharedApplication].applicationState == UIApplicationStateActive) {
                NSError *error = nil;
                [[AVAudioSession sharedInstance] setActive:YES error:&error];
                alcMakeContextCurrent(context);
                laya::JCConch::s_pScriptRuntime->restoreAudio();
            } else {
                resumeOnBecomingActive = true;
            }
        }
    }
    
    if ([notification.name isEqualToString:UIApplicationDidBecomeActiveNotification] && resumeOnBecomingActive) {
        resumeOnBecomingActive = false;
        NSError *error = nil;
        BOOL success = [[AVAudioSession sharedInstance]
                        setCategory: AVAudioSessionCategoryAmbient
                        error: &error];
        if (!success) {
            printf("set audio session failed.\n");
            return;
        }
        [[AVAudioSession sharedInstance] setActive:YES error:&error];
        alcMakeContextCurrent(context);
        laya::JCConch::s_pScriptRuntime->restoreAudio();
    }
}
-(void)update
{
    laya::JCConch::s_pConch->update();
}
-(void) initNetworkListener
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:LayakReachabilityChangedNotification object:nil];
    m_pNetworkListener=[LayaReachability reachabilityForInternetConnection];
    [m_pNetworkListener startNotifier];
}
//------------------------------------------------------------------------------
-(NSString*) getRootCachePath
{
    NSString* sAppDirctory = NSHomeDirectory();
    NSString* sDownloadRootPath = [ NSString stringWithFormat: @"%@/Library/Caches/", sAppDirctory ];
    return sDownloadRootPath;
}
//------------------------------------------------------------------------------
-(NSString*) getResourcePath
{
    return [[NSBundle mainBundle] resourcePath];
}
//------------------------------------------------------------------------------
-(void)destroy
{
    if (m_pTouchFilter != nullptr)
    {
        delete m_pTouchFilter;
        m_pTouchFilter = nullptr;
    }
    if (laya::JCConch::s_pConch)
    {
        laya::JCConch::s_pConch->onAppDestroy();
        laya::JCConch::s_pConch.reset();
    }
    g_pIOSConchRuntime = nil;
}
- (void)dealloc
{
    [self destroy];
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIKeyboardDidShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIDeviceOrientationDidChangeNotification object:nil];
    //移除网络监控
    [m_pNetworkListener stopNotifier];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionInterruptionNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillEnterForegroundNotification object:nil];
}
//------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    auto pScriptRuntime = laya::JCConch::s_pScriptRuntime;
    if(pScriptRuntime)
    {
        pScriptRuntime->jsGC();
    }
}
//------------------------------------------------------------------------------
-(void) initConch
{
    if( m_bEngineInited == false )
    {
        g_nInnerWidth = m_pResolution->x*m_fRetinaValue;
        g_nInnerHeight = m_pResolution->y*m_fRetinaValue;
        gRedistPath = [m_nsRootCachePath cStringUsingEncoding:NSUTF8StringEncoding];
        const char* sResourcePath = [m_nsRootResourcePath cStringUsingEncoding:NSUTF8StringEncoding];
        laya::JCIosFileSource* pRedistFileResource = new laya::JCIosFileSource();
        gResourcePath = sResourcePath;
        gResourcePath += "/";
        gAssetRootPath= gResourcePath+"/cache/";
        pRedistFileResource->Init( gResourcePath.c_str() );
        laya::JCConch::s_pAssetsFiles = pRedistFileResource;
        laya::JCConch::s_pConch.reset(new laya::JCConch());
        laya::JCConch::s_pConchRender->createBackend(m_options);
        laya::JCConch::s_pConchRender->createScreenSurface(m_options.nativeLayer);
		laya::JCConch::s_pConch->onAppStart();
        m_bEngineInited = true;
        NSLog(@"=============onGLReady width=%d,height=%d", g_nInnerWidth, g_nInnerHeight);
        
        // 注册监听键盘弹出的事件
        [[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                                 selector:@selector(keyboardWasShown:)
                                                     name:UIKeyboardWillShowNotification
                                                   object:nil];
        
        // 注册监听键盘弹出完成的事件
        [[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                                 selector:@selector(keyboardWasShownEnd:)
                                                     name:UIKeyboardDidShowNotification
                                                   object:nil];
        /*[[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                                 selector:@selector(changeValue:)
                                                     name:@"changeValue" object:nil];*/
        //[m_pEditBox->m_pEditBox addTarget:m_pEditBoxDelegate action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
        
        if ([[[UIDevice currentDevice] systemVersion] intValue] > 5) {
            [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleInterruption:) name:AVAudioSessionInterruptionNotification object:[AVAudioSession sharedInstance]];
            [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleInterruption:) name:UIApplicationDidBecomeActiveNotification object:nil];
        }
        else {
            //AudioSessionInitialize(NULL, NULL, AudioEngineInterruptionListenerCallback, NULL);
        }

        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(willResignActive:) name:UIApplicationWillResignActiveNotification object:nil];
            [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidEnterBackground:) name:UIApplicationDidEnterBackgroundNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillEnterForeground:) name:UIApplicationWillEnterForegroundNotification object:nil];
        
        // initialize the default values of LayaVideoPlayer
        [LayaVideoPlayer setCurParentView:m_pView withRetianValue:m_fRetinaValue];
        //m_UIEditBoxWX = nil;
        
        //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationDidChanged:)
        //                                             name:UIDeviceOrientationDidChangeNotification object:[UIDevice currentDevice]];
    }
}

- (void)onOrientationChanged:(CGSize)size
{
    //CGRect kRect = [UIScreen mainScreen].bounds;
    m_pResolution->x = size.width;
    m_pResolution->y = size.height;

    int width = m_pResolution->x * m_fRetinaValue;
    int height =m_pResolution->y * m_fRetinaValue;
    if(g_nInnerWidth != width || g_nInnerHeight != height)
    {
        NSLog(@"=============onResize width=%d,height=%d", width, height);
        g_nInnerWidth = width;
        g_nInnerHeight = height;
        g_bGLCanvasSizeChanged = true;
    }
}
-(void)onResize:(CGSize)size;
{
    if (laya::JCConch::s_pConchRender)
        laya::JCConch::s_pConchRender->onScreenSurfaceResize(size.width, size.height);
}
/*- (void)deviceOrientationDidChanged:(NSNotification *)notify
{
    [self updateCanvasSizeIfNeeded];
}*/

//-------------------------------------------------------------------------------
- (int) AddTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount < 10 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == NULL )
            {
                m_vTouch[i] = p_pTouch;
                m_nTouchCount++;
                return i;
            }
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (int) RemoveTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount > 0 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == p_pTouch )
            {
                m_vTouch[i] = NULL;
                m_nTouchCount--;
                return i;
            }
        }
        for( int i = 0; i < 10 ;i++ )
        {
            m_vTouch[i] = NULL;
        }
        m_nTouchCount=0;
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (int) FindTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount > 0 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == p_pTouch )
            {
                return i;
            }
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    int nCount = (int)[allTouches count];
    //如果<=0 就发送down
    if( m_nTouchCount <= 0 )
    {
        //第一个手指
        IOSTouch kIOSTouch;
        kIOSTouch.m_nType = TOUCH_BEGAIN;
        
        //不是第一个手指
        IOSTouch kIOSTouchPD;
        kIOSTouchPD.m_nType = ACTION_POINTER_DOWN;
        
        int nNum = 0;
        for( int i = 0 ; i < nCount ;i++ )
        {
            UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
            CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
            int nID = [self AddTouchToArray:pSysTouch];
            if( nID != -1 )
            {
                //第一个手指是发送 TOUCH_BEGAIN
                if( i == 0 )
                {
                    kIOSTouch.m_nID[0] = nID;
                    kIOSTouch.m_nX[0] = kTouchLocation.x;
                    kIOSTouch.m_nY[0] = kTouchLocation.y;
                }
                //剩下的发送 POINTER_DOWN
                else
                {
                    if( nNum >= TOUCH_EVENT_SIZE )
                    {
                        NSLog(@"can't support touch num > %d",TOUCH_EVENT_SIZE);
                    }
                    kIOSTouchPD.m_nID[nNum] = nID;
                    kIOSTouchPD.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouchPD.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
        }
        //发送 TOUCH_BEGAIN
        kIOSTouch.m_nCount = 1;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
        
        //发送 POINTER_DOWN
        if( nNum > 0 )
        {
            kIOSTouchPD.m_nCount = nNum;
            [self sendTouchEvent:kIOSTouchPD ratio:m_fRetinaValue offset:m_nGLViewOffset];
        }
    }
    else
    {
        IOSTouch kIOSTouch;
        kIOSTouch.m_nType = ACTION_POINTER_DOWN;
        int nNum = 0;
        for( int i = 0 ; i < nCount ;i++ )
        {
            UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
            if( [self FindTouchToArray:pSysTouch ] == -1 )
            {
                CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
                int nID = [self AddTouchToArray:pSysTouch];
                if( nID != -1 )
                {
                    if( nNum >= TOUCH_EVENT_SIZE )
                    {
                        NSLog(@"can't support touch num > %d",TOUCH_EVENT_SIZE);
                    }
                    kIOSTouch.m_nID[nNum] = nID;
                    kIOSTouch.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouch.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
        }
        kIOSTouch.m_nCount = nNum;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
}
//-------------------------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    int nCount = (int)[allTouches count];
    IOSTouch kIosTouch;
    kIosTouch.m_nCount = nCount;
    kIosTouch.m_nType = TOUCHE_MOVED;
    
    for( int i = 0 ; i < nCount ;i++ )
    {
        UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
        CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
        kIosTouch.m_nID[i] = [self FindTouchToArray:pSysTouch];
        kIosTouch.m_nX[i] = kTouchLocation.x;
        kIosTouch.m_nY[i] = kTouchLocation.y;
    }
    [self sendTouchEvent:kIosTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
}
-(void) sendTouchEvent:(IOSTouch&) p_kTouch ratio:(float)p_fRatio offset:(int)p_nViewOffset
{
    if(m_bIgnoreCurEvent)
    {
        return;
    }
    m_pTouchFilter->onTouchEvent( p_kTouch, p_fRatio,p_nViewOffset );
}
//-------------------------------------------------------------------------------
- (void)HandleTouchEnd:(NSSet*)allTouches state:(int)p_nState
{
    int nCount = (int)[allTouches count];
    int nNum = 0;//要发送 pointer_up 的总数
    IOSTouch kIOSTouch;
    kIOSTouch.m_nType = ACTION_POINTER_UP;
    
    IOSTouch kLastTouch;//要发送的end
    kLastTouch.m_nType = TOUCHE_END;
    kLastTouch.m_nCount = 1;
    
    for( int i = 0 ; i < nCount ;i++ )
    {
        UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
        if( p_nState == -1 || pSysTouch.phase == p_nState )
        {
            CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
            int nID = [self RemoveTouchToArray:pSysTouch];
            if( nID != -1 )
            {
                if( m_nTouchCount <= 0 )
                {
                    kLastTouch.m_nID[0] = nID;
                    kLastTouch.m_nX[0] = kTouchLocation.x;
                    kLastTouch.m_nY[0] = kTouchLocation.y;
                    break;
                }
                else
                {
                    kIOSTouch.m_nID[nNum] = nID;
                    kIOSTouch.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouch.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
            else
            {
                NSLog(@"Can't find this touch pointer！！");
            }
        }
    }
    if(nNum>0)
    {
        kIOSTouch.m_nCount = nNum;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
    //发送最后一个
    if( m_nTouchCount <= 0 )
    {
        [self sendTouchEvent:kLastTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
}
//-------------------------------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    [self HandleTouchEnd:allTouches state:3];
}
//-------------------------------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    [self HandleTouchEnd:allTouches state:-1];
}
//-------------------------------------------------------------------------------
-(void) runJsLoop
{
    laya::JSThreadInterface* pJSThread = laya::JCConch::s_pScriptRuntime->m_pScriptThread;
    pJSThread->run(NULL,NULL);
    if(m_bStopEngine)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self reset];
        });
        [m_pNSTimer invalidate];
        CFRunLoopStop( [[NSRunLoop currentRunLoop] getCFRunLoop] );
    }
}
-(void)runFunc:(NSTimer*)timer;
{
    FuncObj* pFunObj = [[timer userInfo] objectForKey:@"fun"];
    if( !m_bStopEngine && pFunObj && pFunObj->m_Fuction )
    {
        pFunObj->m_Fuction();
    }
    [timer invalidate];
    timer=nil;
}
//-------------------------------------------------------------------------------
- (void)networkStateChange
{
    int nState = [self checkNetworkState];
    auto pScriptRuntime = laya::JCConch::s_pScriptRuntime;
    if(pScriptRuntime)
    {
        pScriptRuntime->onNetworkChanged(nState);
    }
}
-(int)checkNetworkState
{
    // 1.检测手机是否能上网络(WIFI\3G\2.5G\4G)
    LayaReachability *conn = [LayaReachability reachabilityForInternetConnection];
    if ([conn currentReachabilityStatus] == NotReachable)
    {
        NSLog(@">>>>>>>checkNetworkState 没有网络");
        return laya::NET_NO;
    }
    else
    {
        NSLog(@">>>>>>>checkNetworkState 有网络");
        return laya::NET_YES;
    }
}
//-------------------------------------------------------------------------------
-(void) setScreenOrientation:(int)p_nType
{
    
    if (@available(iOS 16.0, *)) {
        
        switch(p_nType)
        {
            case 0://landscape 16
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight;
                break;
            case 1://portrait 2
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortrait;
                break;
            case 8://reverse_landscape 8
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeLeft;
                break;
            case 9://reverse_portrait 4
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortraitUpsideDown;
                break;
            case 4://sensor all
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
                break;
            case 10://full sensor all
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
                break;
            case 2://user full
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
                break;
            case 6://sensor_landscape
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight | UIInterfaceOrientationMaskLandscapeLeft;
                break;
            case 7://sensor_portrait
                laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
                break;
            default://behind nosensor
                break;
        }
        
        [UIApplication.sharedApplication.delegate.window.rootViewController setNeedsUpdateOfSupportedInterfaceOrientations];
        NSSet<UIScene *> *connectedScenes = [[UIApplication sharedApplication] connectedScenes];
        if (connectedScenes != nil) {
            NSArray *array = [connectedScenes allObjects];
            if (array != nil && array.count > 0) {
                UIWindowScene *scene = (UIWindowScene *)array[0];
                if (scene != nil) {
                    UIWindowSceneGeometryPreferencesIOS *geometryPreferences = [[UIWindowSceneGeometryPreferencesIOS alloc] initWithInterfaceOrientations: laya::g_kSystemConfig.m_nOrientationType];
                    [scene requestGeometryUpdateWithPreferences:geometryPreferences
                                                   errorHandler:^(NSError * _Nonnull error) {
                        //NSAssert(NO, [NSString stringWithFormat:@"rotate screen error：%@", error]);
                    }];
                }
            }
        }
        return;
    }
    
    //强制调一次AppDelegate supportedInterfaceOrientationsForWindow，解决调试转屏失败的bug
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        UITextField *textField = [[UITextField alloc] init];
        [UIApplication.sharedApplication.delegate.window.rootViewController.view addSubview:textField];
        [textField becomeFirstResponder];
        [textField resignFirstResponder];
        [textField removeFromSuperview];
    });
    
    //防止未锁定屏幕方向时旋转失效
    NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationUnknown];
    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    
    switch(p_nType)
    {
        case 0://landscape 16
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
            
        }
            break;
        case 1://portrait 2
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortrait;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortrait];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 8://reverse_landscape 8
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeLeft;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 9://reverse_portrait 4
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortraitUpsideDown;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortraitUpsideDown];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 4://sensor all
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 10://full sensor all
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 2://user full
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 6://sensor_landscape
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight | UIInterfaceOrientationMaskLandscapeLeft;
            if (!UIInterfaceOrientationIsLandscape([UIApplication sharedApplication].statusBarOrientation))
            {
                NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
                [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
            }
        }
            break;
        case 7://sensor_portrait
        {
            laya::g_kSystemConfig.m_nOrientationType = UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
            
            if (!UIInterfaceOrientationIsPortrait([UIApplication sharedApplication].statusBarOrientation))
            {
                NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortrait];
                [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
            }
            break;
        }
        default://behind nosensor
            break;
    }
}
/*-(void)updateResolution
{
    CGRect kRect = [UIScreen mainScreen].bounds;
    int nOrientation = UIDevice.currentDevice.orientation;
    //竖屏
    if( nOrientation == UIInterfaceOrientationPortrait ||
       nOrientation ==  UIInterfaceOrientationPortraitUpsideDown )
    {
        m_pResolution->x = MIN( kRect.size.width , kRect.size.height );
        m_pResolution->y = MAX( kRect.size.width , kRect.size.height );
    }
    //横屏
    else if( nOrientation == UIInterfaceOrientationLandscapeLeft ||
            nOrientation ==  UIInterfaceOrientationLandscapeRight )
    {
        m_pResolution->x = MAX( kRect.size.width , kRect.size.height );
        m_pResolution->y = MIN( kRect.size.width , kRect.size.height );
    }

}*/
-(void)reset
{
    if(m_pLayaAlert)
    {
        [m_pLayaAlert reset];
    }
    if (m_pReflection){
        [m_pReflection clearReflectionObjects];
    }
}

-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithClass:cls methodName:name ret:retObj];
}

-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithClassName:cls methodName:name ret:retObj];
}
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithObject:obj methodName:name ret:retObj];
}
- (void)applicationDidEnterBackground:(NSNotification *)notification {
    if (laya::JCConch::s_pConch) {
		laya::JCConch::s_pConch->onAppPause();
	}
    [[LayaDeviceSensor GetInstance] stop];
}

- (void)applicationWillEnterForeground:(NSNotification *)notification {
	if (laya::JCConch::s_pConch) {
		laya::JCConch::s_pConch->onAppResume();
	}
    [[LayaDeviceSensor GetInstance] start];
}

- (void)willResignActive:(NSNotification *)notification {
    //CToObjectCOnBlur();
}

-(void)runJS:(NSString*)script{
    
    const char* pScript = [script UTF8String];
    if (pScript){
        auto pFunction = std::bind(&CToObjectCRunJS,std::string(pScript));
        laya::JCConch::s_pScriptRuntime->postToJS(pFunction);
    }
}
-(void)setFileDataHandler:(HandleFileData)func
{
    laya::setFileDataHandler(func);
}
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey, id> *)info
{
#if 0
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    image = [self compressOriginalImage:image];
    NSString * extention = nil;
    if (image != nil) {
        if (picker.sourceType == UIImagePickerControllerSourceTypeCamera) {
            UIImageWriteToSavedPhotosAlbum(image, self, nil, nil);
        }
        
        NSData * data = nil;
            
        if (UIImagePNGRepresentation(image) == nil) {
            data = UIImageJPEGRepresentation(image, 0.8);
            extention = @".jpg";
        }
        else {
            data = UIImagePNGRepresentation(image);
            extention = @".png";
        }
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString* tempPath = [[NSString alloc] initWithUTF8String:LAYA_NATIVE_FILE_CACHE_TMP_PATH.c_str()];
        NSString* fileName = [[NSString alloc] initWithFormat:@"%@%@", conchRuntime.uuid, extention];
        NSString* imagePath = [[NSString alloc] initWithFormat:@"%@/%@",tempPath, fileName];
        [fileManager createFileAtPath:imagePath contents:data attributes:nil];

        NSMutableDictionary* dic = [[NSMutableDictionary alloc] init];
        {
            NSString* imagePathWX = [[NSString alloc] initWithFormat:@"wxfile://tmp/%@",fileName];
            {
                NSMutableArray* arrayFilePaths = [[NSMutableArray alloc] init];
                [arrayFilePaths addObject:imagePathWX];
                [dic setValue:arrayFilePaths forKey:@"tempFilePaths"];
            }
            {
                NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
                [item setValue:imagePathWX forKey:@"path"];
                [item setValue:@222 forKey:@"size"];
        
                NSMutableArray* arrayFiles = [[NSMutableArray alloc] init];
                [arrayFiles addObject:item];
                
                [dic setValue:arrayFiles forKey:@"tempFiles"];
            }
        }
        NSError* pError = nil;
        NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&pError];
        if(pError)
        {
            return;
        }
        laya::JSLayaNative::getInstance()->onCompleteCallJSFunction(1, [[[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding] UTF8String]);
        [picker dismissViewControllerAnimated:YES completion:nil];
    }
#endif
}
+ (NSString *)uuid
{
    CFUUIDRef uuid_ref = CFUUIDCreate(NULL);
    CFStringRef uuid_string_ref= CFUUIDCreateString(NULL, uuid_ref);
    NSString *uuid = [NSString stringWithString:(__bridge NSString *)uuid_string_ref];
    CFRelease(uuid_ref);
    CFRelease(uuid_string_ref);
    return [uuid lowercaseString];
}
-(UIImage *)compressOriginalImage:(UIImage *)image
{
    CGSize imageSize = image.size;
    CGFloat targetWidth = imageSize.width;
    CGFloat targetHeight = imageSize.height;
    CGFloat originalWidth = imageSize.width;
    CGFloat originalHeight = imageSize.height;
    
    if (originalWidth > 2048 || originalHeight >2048)
    {
        if (originalWidth > originalHeight)
        {
            targetWidth = 1024;
            targetHeight = targetWidth * originalHeight /originalWidth;
            
        }
        else
        {
            targetHeight = 1024;
            targetWidth = targetHeight * originalWidth / originalHeight;
        }
    }
    UIGraphicsBeginImageContext(CGSizeMake(targetWidth, targetHeight));
    [image drawInRect:CGRectMake(0,0,targetWidth, targetHeight)];
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}
-(void)saveImageToAlbum:(NSString*)imgPath
{
    UIImage* img = [UIImage imageWithContentsOfFile:imgPath];
    UIImageWriteToSavedPhotosAlbum(img, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}

#pragma mark 保存到相册
-(void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo {
#if 0
    NSString *msg = nil ;
    if(error){
        msg = @"保存图片失败" ;
        laya::JSLayaNative::getInstance()->onSaveImageComplete(0);
    }else{
        msg = @"保存图片成功" ;
        laya::JSLayaNative::getInstance()->onSaveImageComplete(1);
    }
#endif
}
-(void)chooseImage:(int)count sizeType:(NSString*)sizeType sourceType:(NSString*)sourceType
{
#if 0
    if ([sourceType isEqualToString:@"camera"]) {
        if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
            UIImagePickerController* pickController = [[UIImagePickerController alloc] init];
            pickController.sourceType = UIImagePickerControllerSourceTypeCamera;
            //pickController.mediaTypes = @[@"public.image"];
            pickController.delegate = self;
            [UIApplication.sharedApplication.delegate.window.rootViewController presentViewController:pickController animated:YES completion:nil];
        }
    }
    else {
        if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) {
            UIImagePickerController* pickController = [[UIImagePickerController alloc] init];
            pickController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            //pickController.mediaTypes = @[@"public.image"];
            pickController.delegate = self;
            [UIApplication.sharedApplication.delegate.window.rootViewController presentViewController:pickController animated:YES completion:nil];
        }
    }
#endif
}
-(void)showEditBoxWX:(NSString*)defaultValue maxLength:(int)maxLength multiple:(bool)multiple confirmHold:(bool)confirmHold confirmType:(NSString*)confirmType prompt:(NSString*)prompt promptColor:(NSString*)promptColor inputType:(NSString*)inputType
{
#if 0
    if (m_UIEditBoxWX == nil) {
        m_UIEditBoxWX = [[UIEditBoxWX alloc] initWithMultiple:multiple];
    }
    else {
        [m_UIEditBoxWX clean];
        m_UIEditBoxWX = [[UIEditBoxWX alloc] initWithMultiple:multiple];
    }
    m_UIEditBoxWX.defaultText = defaultValue;
    m_UIEditBoxWX.maxLength = maxLength;
    m_UIEditBoxWX.confirmHold = confirmHold;
    m_UIEditBoxWX.confirmType = confirmType;
    m_UIEditBoxWX.prompt = prompt;
    m_UIEditBoxWX.promptColor = promptColor;
    m_UIEditBoxWX.inputType = inputType;
    [m_UIEditBoxWX becomeFirstResponder];
#endif
}

-(void)hideEditBoxWX
{
#if 0
    if (m_UIEditBoxWX != nil) {
        [m_UIEditBoxWX hide];
        m_UIEditBoxWX = nil;
    }
#endif
}
-(void)alert:(NSString*)sInfo
{
    [[conchRuntime GetIOSConchRuntime]->m_pLayaAlert alert:[sInfo UTF8String]];
}
+(UIInterfaceOrientationMask)getOrientationMask
{
    return laya::g_kSystemConfig.m_nOrientationType;
}
- (NSUInteger)parseInterfaceOrientations:(NSArray*)orientations
{
    NSUInteger ret = 0;
    if (orientations != nil) {
        NSEnumerator* enumerator = [orientations objectEnumerator];
        NSString* orientationString;

        while (orientationString = [enumerator nextObject]) {
            if ([orientationString isEqualToString:@"UIInterfaceOrientationPortrait"]) {
                ret = ret | (1 << UIInterfaceOrientationPortrait);
            } else if ([orientationString isEqualToString:@"UIInterfaceOrientationPortraitUpsideDown"]) {
                ret = ret | (1 << UIInterfaceOrientationPortraitUpsideDown);
            } else if ([orientationString isEqualToString:@"UIInterfaceOrientationLandscapeLeft"]) {
                ret = ret | (1 << UIInterfaceOrientationLandscapeLeft);
            } else if ([orientationString isEqualToString:@"UIInterfaceOrientationLandscapeRight"]) {
                ret = ret | (1 << UIInterfaceOrientationLandscapeRight);
            }
        }
    }
    return ret;
}
@end
