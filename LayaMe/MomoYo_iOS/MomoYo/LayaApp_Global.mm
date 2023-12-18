//
//  LayaApp.m
//  WKWebview
//
//  Created by Alex on 2019/9/5.
//  Copyright © 2019 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LayaApp.h"
#import "iap/IAPManager.h"
#import "AppDelegate.h"
#import "conchRuntime.h"
#import <UserNotifications/UserNotifications.h>
#import "ViewController.h"
#include <sys/sysctl.h>
//const NSString* url = @"https://lv.layabox.com/dev/momoyoglobal/version.json?amazon=1&clientTag=1";//开发测试地址
//const NSString* url = @"https://lv.layabox.com/lfl/momoyoglobal/version.json?amazon=1&clientTag=1";//测试地址
//const NSString* url = @"https://lv.layabox.com/test/layadev3.0/momoyoglobal/version.json?amazon=1&clientTag=1";//测试地址
//const NSString* url = @"https://lv.layabox.com/test/layame/momoyoglobal/version.json?amazon=1";//稳定地址
//const NSString* url = @"https://client.momoyogame.com/app/20221227/ios/momoyoglobal/version.json?amazon=1";//稳定地址amazon
const NSString* url = @"http://127.0.0.1:13999/version.json?amazon=1";//稳定地址amazon
const NSString* qudao = @"momoyoglobal_app";
const NSString* channel = @"AppStore";
NSInteger qudaoID = -1;
long long g_secondTimeLoginMs = 0;
static std::string s_ticket;
@implementation LayaApp

+(long long)getCurrentMs {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init] ;
    
    [formatter setDateStyle:NSDateFormatterMediumStyle];
    
    [formatter setTimeStyle:NSDateFormatterShortStyle];
    
    [formatter setDateFormat:@"YYYY-MM-dd HH:mm:ss SSS"];
    NSTimeZone* timeZone = [NSTimeZone timeZoneWithName:@"Asia/Shanghai"];
    [formatter setTimeZone:timeZone];
    NSDate *datenow = [NSDate date];
    
   return (long long)([datenow timeIntervalSince1970] * 1000);
}
+(void)loginQQ{
}

@end

