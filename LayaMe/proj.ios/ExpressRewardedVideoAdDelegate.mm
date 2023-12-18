//
//  ExpressRewardedVideoAdDelegate.m
//  LayaMe
//
//  Created by Alex on 2020/4/26.
//  Copyright © 2020 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExpressRewardedVideoAdDelegate.h"
#import "ViewController.h"
#import "BDHub.h"

@implementation ExpressRewardedVideoAdDelegate

typedef enum
{
    RewardedVideoState_Invalid = -1,
    RewardedVideoState_DidPlayFinish_Sucess,
    RewardedVideoState_DidPlayFinish_Failed,
} RewardedVideoState;

static ExpressRewardedVideoAdDelegate* _instance;
static BUNativeExpressRewardedVideoAd* _ad;
static NSString* _adData;
static RewardedVideoState _state = RewardedVideoState_Invalid;



+(ExpressRewardedVideoAdDelegate*) instance{
    if (nil == _instance){
        _instance = [[ExpressRewardedVideoAdDelegate alloc]init];
    }
    return _instance;
}

+(BUNativeExpressRewardedVideoAd*) AD{
    return _ad;
}

+(void)setAD:(BUNativeExpressRewardedVideoAd *)ad{
    _ad = ad;
}
+(void)setADData:(NSString *)adData{
    _adData = adData;
}

//加载成功
-(void)nativeExpressRewardedVideoAdDidLoad:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
    NSLog(@"nativeExpressRewardedVideoAdDidLoad");
}

//加载失败
-(void)nativeExpressRewardedVideoAd:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd didFailWithError:(NSError *)error{
    [BDHub.instance rewardVideoCallback:-1];
    NSLog(@"nativeExpressRewardedVideoAd");
}
- (void)nativeExpressRewardedVideoAdCallback:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd withType:(BUNativeExpressRewardedVideoAdType)nativeExpressVideoType
{
    NSLog(@"nativeExpressRewardedVideoAdCallback");
}
//缓存成功
-(void)nativeExpressRewardedVideoAdDidDownLoadVideo:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
    NSLog(@"nativeExpressRewardedVideoAdDidDownLoadVideo");
    _state = RewardedVideoState_Invalid;
    [rewardedVideoAd showAdFromRootViewController:[ViewController GetIOSViewController]];

    NSLog(@"nativeExpressRewardedVideoAdDidDownLoadVideo");
}
//渲染成功
-(void)nativeExpressRewardedVideoAdViewRenderSuccess:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
    //[rewardedVideoAd showAdFromRootViewController:ViewController.instance];
    NSLog(@"nativeExpressRewardedVideoAdViewRenderSuccess");
}
//渲染失败
-(void)nativeExpressRewardedVideoAdViewRenderFail:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd error:(NSError *)error{
    [BDHub.instance rewardVideoCallback:-1];
    //[BDHub.instance statLoadFail:_adData message:error.description];
    NSLog(@"nativeExpressRewardedVideoAdViewRenderFail");
}
- (void)nativeExpressRewardedVideoAdWillVisible:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd
{
    NSLog(@"nativeExpressRewardedVideoAdWillVisible");
}
//广告显示
-(void)nativeExpressRewardedVideoAdDidVisible:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
     NSLog(@"nativeExpressRewardedVideoAdDidVisible");
}

//广告即将关闭
-(void)nativeExpressRewardedVideoAdWillClose:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
    NSLog(@"nativeExpressRewardedVideoAdWillClose");
}
//广告关闭
-(void)nativeExpressRewardedVideoAdDidClose:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd{
    
    if (_state == RewardedVideoState_DidPlayFinish_Sucess)
    {
        [BDHub.instance rewardVideoCallback:0];
        //[BDHub.instance statPlayFinish:_adData];
    }
    else if (_state == RewardedVideoState_DidPlayFinish_Failed)
    {
        [BDHub.instance rewardVideoCallback:-2];
    }
    NSLog(@"nativeExpressRewardedVideoAdDidClose");
}
//广告点击
-(void)nativeExpressRewardedVideoAdDidClick:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd
{
    NSLog(@"nativeExpressRewardedVideoAdDidClick");
}
//广告跳过
-(void)nativeExpressRewardedVideoAdDidClickSkip:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd
{
    NSLog(@"nativeExpressRewardedVideoAdDidClickSkip");
}
//播放完成
-(void)nativeExpressRewardedVideoAdDidPlayFinish:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd didFailWithError:(NSError *)error
{
    if (nil == error)
    {
        _state = RewardedVideoState_DidPlayFinish_Sucess;
    }
    else
    {
       _state = RewardedVideoState_DidPlayFinish_Failed;
    }
    NSLog(@"nativeExpressRewardedVideoAdDidPlayFinish");
}
- (void)nativeExpressRewardedVideoAdServerRewardDidSucceed:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd verify:(BOOL)verify
{
    NSLog(@"nativeExpressRewardedVideoAdServerRewardDidSucceed");
}
- (void)nativeExpressRewardedVideoAdServerRewardDidFail:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd
{
      NSLog(@"nativeExpressRewardedVideoAdServerRewardDidFail");
}

- (void)nativeExpressRewardedVideoAdDidCloseOtherController:(BUNativeExpressRewardedVideoAd *)rewardedVideoAd interactionType:(BUInteractionType)interactionType
{
      NSLog(@"nativeExpressRewardedVideoAdDidCloseOtherController");
}
@end
