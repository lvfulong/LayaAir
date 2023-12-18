//
//  BDHub.m
//  SnowFight
//
//  Created by Alex on 2019/9/20.
//  Copyright © 2019 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BDHub.h"
//#import "RequestTool.h"
//#import <BUAdSDK/BUAdSDK.h>
//#import "ByteDance/BannerAdViewDelegate.h"
//#import "ByteDance/RewardedVideoAdDelegate.h"
//#import "ByteDance/InterstitialAdDelegate.h"
#import "ExpressRewardedVideoAdDelegate.h"
//#import "Tencent/GDTBannerViewDelegate.h"
//#import "Tencent/GDTRewardedVideoDelegate.h"
//#import "Tencent/GDTInterstitialDelegate.h"
//#import "GDTSDKConfig.h"
#import "ViewController.h"
#import "LayaApp.h"

#import <AdSupport/AdSupport.h>

@interface SlotInfo : NSObject
@property (nonatomic,strong) NSString* SponsorKey;
@property (nonatomic,strong) NSString* Key;
@property (nonatomic,assign) int Type;
@end
@implementation SlotInfo
@end

static NSString* BDHUB_BASEURL=@"https://master-laya-bdhub.layabox.com/";

static const int SPONSOR_TYPE_BYTEDANCE = 1;
static const int SPONSOR_TYPE_TENCENT = 2;
static NSMutableDictionary* slots;

@implementation BDHub

static BDHub* _instance;
BOOL _inited = NO;
NSString* _idfa;
+(BDHub*)instance{
    return _instance;
}
-(instancetype)initWithAppId:(NSString*) appId{
    
    self = [super init];
    if (self){
        _appId = appId;
        _instance = self;
        _idfa = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
        NSDictionary* params = [[NSDictionary alloc]initWithObjectsAndKeys:
            @"info",@"a",
            @"app",@"m",
            _appId,@"appId",
            @"1", @"platform"
        , nil];
    }
    
    return self;
}

-(SlotInfo*)getSlotInfo:(NSString*)slotId{
    return [slots valueForKey:slotId];
}



-(void)showRewardVideo:(NSString*)type data: (NSString*)strData{
    [BUAdSDKManager setAppID:@"5062585"];
    BURewardedVideoModel* model = [[BURewardedVideoModel alloc] init];
    model.userId = _idfa;
    //ExpressRewardedVideoAdDelegate.AD = [[BUNativeExpressRewardedVideoAd alloc] initWithSlotID:slot.Key rewardedVideoModel:model];
    ExpressRewardedVideoAdDelegate.AD = [[BUNativeExpressRewardedVideoAd alloc] initWithSlotID:@"945151952" rewardedVideoModel:model];
    //ExpressRewardedVideoAdDelegate.ADData = data;
    ExpressRewardedVideoAdDelegate.AD.delegate = ExpressRewardedVideoAdDelegate.instance;
    [ExpressRewardedVideoAdDelegate.AD loadAdData];
}

-(void)rewardVideoCallback:(int)code{
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInt:code], @"code"
                          , nil];
    
    [LayaApp callback:@"showRewardVideo:data:" data:dict];
}
-(NSDictionary*)parseJSON:(NSString*)str{
    NSError* error = nil;
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    return [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
}


@end
