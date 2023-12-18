//
//  BDHub.h
//  SnowFight
//
//  Created by Alex on 2019/9/20.
//  Copyright © 2019 LayaBox. All rights reserved.
//

#import <Foundation/NSObject.h>
@interface BDHub: NSObject
{
    NSString* _appId;
}
-(instancetype)initWithAppId:(NSString*)appId;

+(BDHub*)instance;

-(void)showRewardVideo:(NSString*) type data: (NSString*)strData;

-(void)rewardVideoCallback:(int)code;
@end
