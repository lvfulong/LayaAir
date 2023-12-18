//
//  ExpressRewardedVideoAdDelegate.h
//  LayaMe
//
//  Created by Alex on 2020/4/26.
//  Copyright © 2020 LayaBox. All rights reserved.
//

#import <BUAdSDK/BUAdSDK.h>

@interface ExpressRewardedVideoAdDelegate : NSObject<BUNativeExpressRewardedVideoAdDelegate>
+(ExpressRewardedVideoAdDelegate*) instance;
+(BUNativeExpressRewardedVideoAd*) AD;
+(void)setAD:(BUNativeExpressRewardedVideoAd*)ad;
+(void)setADData:(NSString*)adData;
@end
