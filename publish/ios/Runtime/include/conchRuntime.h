/**
 @file			conchRuntime.h
 @brief         runtime类
 @author		James
 @version		1.0
 @date			2015_8_28
 @company       LayaBox
 */
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <functional>

typedef char* (*HandleFileData)(const char* data, int& len);

class JCTouchFilter;
@class LayaReachability;
@class JCMp3Player;
@class LayaEditBox;
@class LayaEditBoxDelegate;
@class LayaAlert;
@class Reflection;

@interface FuncObj : NSObject
{
@public
    std::function<void(void)> m_Fuction;
};
-(id)init:(std::function<void(void)>)func;
@end

@interface conchRuntime : NSObject<UIImagePickerControllerDelegate>
{
@public
    
    UIView*                    m_pView;
    
    
    bool                        m_bEngineInited;
    
    //-----------------------------------------------------------------
    //touch用的
    JCTouchFilter*              m_pTouchFilter;
    
    UITouch*                    m_vTouch[10];
    
    int                         m_nTouchCount;
    
    //-----------------------------------------------------------------
    //editBox相关的
    int                         m_nGLViewOffset;        //因为editBox导致得移动
    
    LayaEditBox*                m_pEditBox;             //EditBox
    
    LayaEditBoxDelegate*        m_pEditBoxDelegate;     //eiditBoxDelegate
    
    //-----------------------------------------------------------------
    
    JCMp3Player*                m_pMp3Player;           //MP3播放器
    
    LayaReachability*           m_pNetworkListener;     //网络监听
    
    //-----------------------------------------------------------------
    
    float                       m_fRetinaValue;         //Retina的缩放比例
    
    
    
    CGPoint*                    m_pResolution;          //当前分辨率
    
    float                       m_fIOSVersion;          //ios设备的版本
    
    NSTimer*                    m_pNSTimer;             //定时器
    
    bool                        m_bIgnoreCurEvent;
    
    LayaAlert*                  m_pLayaAlert;

    Reflection*                 m_pReflection;
    
    int                         m_nCurrentOrientation;
    bool                        m_bStopEngine;
    
    NSRunLoop*                  m_pJSRunLoop;
    
    CADisplayLink*              m_displayLink;
}


//全局的MainView
+(conchRuntime*)GetIOSConchRuntime;
-(id)initWithFrame:(CGRect)frame URL:(NSString*)pUrl;
-(void)initConch;
-(void)didReceiveMemoryWarning;
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)update;
-(int)checkNetworkState;
-(void)setScreenOrientation:(int)p_nType;
-(void)reset;
-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)runJS:(NSString*)script;
-(void)setFileDataHandler:(HandleFileData)func;
-(void)destroy;
-(void)runJsLoop;
-(void)runFunc:(NSTimer*)timer;
-(void)chooseImage:(int)count sizeType:(NSString*)sizeType sourceType:(NSString*)sourceType;
-(void)showKeyboard:(NSString*)defaultValue maxLength:(int)maxLength multiple:(bool)multiple confirmHold:(bool)confirmHold confirmType:(NSString*)confirmType prompt:(NSString*)prompt promptColor:(NSString*)promptColor inputType:(NSString*)inputType;
-(void)hideKeyboard;
-(void)onOrientationChanged:(CGSize)size;
-(void)onResize:(CGSize)size;
-(void)alert:(NSString*)sInfo;
+(UIInterfaceOrientationMask)getOrientationMask;
@end
