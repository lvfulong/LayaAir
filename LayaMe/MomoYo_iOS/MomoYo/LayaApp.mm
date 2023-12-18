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
#import <GameCreatorLayaSDK/GameCreatorLayaSDK.h>
#import <UserNotifications/UserNotifications.h>
#import "ViewController.h"
#include <sys/sysctl.h>
//const NSString* url = @"https://lv.layabox.com/test/layadev3.0/momoyo/version.json?clientTag=1";//测试地址
const NSString* url = @"https://lv.layabox.com/test/layame/momoyo/version.json?dummy=0";//稳定地址
//const NSString* url = @"https://lv.layabox.com/lfl/momoyo/version.json?clientTag=1";
const NSString* qudao = @"momoyo_app";
const NSString* channel = @"AppStore";
NSInteger qudaoID = -1;
NSUInteger GCEnvironment = GCLayaSDKEnvironmentCN;

@implementation LayaApp

+(void)loginQQ{
}
+(void)login:(NSString*)type {
}
+(void)sendCodeForLogin:(NSString*)kPhone {
    GCLSendCodeRequestModel *model = [GCLSendCodeRequestModel new];
    model.mobile = kPhone;
    model.smsCodeType = GCLSMSCodeScenarioQuickLogin;
    [[GCLayaAccountManager sharedInstance] sendSMSCodeWithModel:model
                                                         completion:^(NSNumber * _Nullable retryTime, UIImage * _Nullable captchaImage, NSError * _Nullable error) {
        if (!error) {
          //获取验证码成功
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithInt:0], @"errCode"
                                     , nil];
               [LayaApp callback:@"sendCodeForLogin:" data:dict];
        } else {
           //获取验证码失败
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithInt:-1], @"errCode"
                                  , [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                                  , [error localizedDescription], @"errMsg"
                                     , nil];
               [LayaApp callback:@"sendCodeForLogin:" data:dict];
        }
    }];
}
+(void)quickLogin:(NSString*)kPhone smsCode:(NSString*)SMSCode {
    [[GCLayaAccountManager sharedInstance] quickLogin:kPhone SMSCode:SMSCode jsonObjCompletion:^(GCLayaAccountResponseModel * _Nullable resp, NSError * _Nullable error) {
            if (!error) {
             // "登录成功"
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithInt:0], @"errCode"
                                         , nil];
                   [LayaApp callback:@"quickLogin:smsCode:" data:dict];
            } else {
             // error.description
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithInt:-1], @"errCode"
                                      , [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                                      , [error localizedDescription], @"errMsg"
                                         , nil];
                   [LayaApp callback:@"quickLogin:smsCode:" data:dict];
            }
          }];
}
+(void)secondTimeLogin:(NSString*)url birthday:(NSString*)birthday  {
        if ([[GCLayaAccountManager sharedInstance] isLogin]) {
                //NSString * _Nonnull url = @"http://develop-ohayou-maker.layabox.com/auth/login";
                NSString* fullUrl = [NSString stringWithFormat:@"%@/auth/login", url];
                NSLog(@"secondTimeLogin:%@ %@", fullUrl, birthday);
                NSMutableDictionary *params = [NSMutableDictionary dictionary];
                //[params setValue:@"937ff5d690c1d2fc47a9b09f849424b3c50978ce" forKey:@"code"];
                [params setValue:birthday forKey:@"birthday"];
                [params setValue:@(2) forKey:@"sp_id"];//国内
                //[params setValue:@(3) forKey:@"sp_id"];//海外
                [params setValue:@"laya_0012" forKey:@"game_id"];
                
                NSDictionary *header = @{@"Content-Type" : @"application/x-www-form-urlencoded"};
                [GCLayaTTNetManager requestURLString: fullUrl
                                              params:params
                                              method:@"POST"
                                    needCommonParams:YES
                                         headerField:header success:^(NSDictionary * _Nonnull JSONObj, NSDictionary * _Nonnull responseHeaderFields) {
                    NSLog(@"请求成功 JSONObj:%@ responseHeaderFields:%@", JSONObj, responseHeaderFields);
                    
                    
                    NSError* pError = nil;
                    NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:JSONObj options:NSJSONWritingPrettyPrinted error:&pError];
                    if(pError)
                    {
                        return;
                    }
                    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                          [NSNumber numberWithInt:2], @"sp_id",
                                          @"laya_0012",@"game_id",
                                             [NSNumber numberWithInt:0], @"errCode"
                                            , [[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding], @"data"
                                             , nil];
                       [LayaApp callback:@"secondTimeLogin:birthday:" data:dict];
                } failure:^(NSError * _Nonnull error, NSDictionary * _Nullable responseHeaderFields) {
                    NSLog(@"请求失败 error:%@ responseHeaderFields:%@", error, responseHeaderFields);
                    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                             [NSNumber numberWithInt:-1], @"errCode"
                                          , [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                                          , [error localizedDescription], @"errMsg"
                                             , nil];
                       [LayaApp callback:@"secondTimeLogin:birthday:" data:dict];
                }];
        }
        else {
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithInt:-1], @"errCode"
                                  , @"", @"errMsg"
                                     , nil];
               [LayaApp callback:@"secondTimeLogin:birthday:" data:dict];
        }
}
+(void)logout:(NSString*)type {
    if ([type isEqualToString:@"gamecreator"]) {
        [[GCLayaAccountManager sharedInstance] logout:^(BOOL success, NSError * _Nullable error) {
            if (!error) {
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithInt:0], @"errCode"
                                   
                                         , nil];
                   [LayaApp callback:@"logout:" data:dict];
            }
            else {
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithInt:-1], @"errCode"
                                      , [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                                      , [error localizedDescription], @"errMsg"
                                         , nil];
                   [LayaApp callback:@"logout:" data:dict];
            }
         }];
    }
}
+(void)isLogin:(NSString*)type {
    if ([type isEqualToString:@"gamecreator"]) {
        BOOL isLogin = [[GCLayaAccountManager sharedInstance] isLogin];
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: isLogin ? @YES : @NO, @"isLogin", nil];
        [LayaApp callback:@"isLogin:" data:dict];
    
    }
}

+(NSDictionary*)parseJSON:(NSString*)str{
    NSError* error = nil;
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    return [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
}

+(void)callback:(NSString*)method data:(NSObject*) data{
    NSError* error = nil;
    
    //NSData* jsonData = [NSJSONSerialization dataWithJSONObject:data options:0 error:&error];
    //NSString* jsonStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    //[[conchRuntime GetIOSConchRuntime] callbackToJSWithClass:self.class methodName:method ret:jsonStr];
    [[conchRuntime GetIOSConchRuntime] callbackToJSWithClass:self.class methodName:method ret:data];
}

+(void)exit {
    abort();
}

+(void)isNotificationsEnabled
{
    __block BOOL bAreNotificationsEnabled = NO;
    if (@available(iOS 10.0, *)) {
  
        [[UNUserNotificationCenter currentNotificationCenter] getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings * _Nonnull settings) {
                if (settings.authorizationStatus == UNAuthorizationStatusNotDetermined)
                {
                    NSLog(@"未选择");
                    bAreNotificationsEnabled = NO;
                }else if (settings.authorizationStatus == UNAuthorizationStatusDenied){
                    NSLog(@"未授权");
                    bAreNotificationsEnabled = NO;
                }else if (settings.authorizationStatus == UNAuthorizationStatusAuthorized){
                    NSLog(@"已授权");
                    bAreNotificationsEnabled = YES;
                }
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: bAreNotificationsEnabled ? @YES : @NO, @"isNotificationsEnabled", nil];
                [LayaApp callback:@"isNotificationsEnabled" data:dict];
            }];
        } else {
            if ([[UIApplication sharedApplication] currentUserNotificationSettings].types == 0) {
                bAreNotificationsEnabled = NO;
            }
            else {
                bAreNotificationsEnabled = YES;
            }
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: bAreNotificationsEnabled ? @YES : @NO, @"isNotificationsEnabled", nil];
            [LayaApp callback:@"isNotificationsEnabled" data:dict];
        }
}
+(void)openNotificationSettings
{
    NSURL *url1 = [NSURL URLWithString:@"App-Prefs:root=NOTIFICATIONS_ID"];
    NSURL *url2 = [NSURL URLWithString:UIApplicationOpenSettingsURLString];
    if (@available(iOS 11.0, *)) {
        if ([[UIApplication sharedApplication] canOpenURL:url2]){
            [[UIApplication sharedApplication] openURL:url2 options:@{} completionHandler:^(BOOL success) {
                NSLog(@"openNotificationSettings completion");
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: success ? @YES : @NO, @"success", nil];
                [LayaApp callback:@"openNotificationSettings" data:dict];
            }];
        }
    } else {
        if ([[UIApplication sharedApplication] canOpenURL:url1]){
            if (@available(iOS 10.0, *)) {
                [[UIApplication sharedApplication] openURL:url1 options:@{} completionHandler:^(BOOL success) {
                    NSLog(@"openNotificationSettings completion");
                    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: success ? @YES : @NO, @"success", nil];
                    [LayaApp callback:@"openNotificationSettings" data:dict];
                }];
            } else {
                [[UIApplication sharedApplication] openURL:url1];
                NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: @YES, @"success", nil];
                [LayaApp callback:@"openNotificationSettings" data:dict];
            }
        }
    }
}
+(void)share:(NSString*)type data:(NSString*)data {
    NSDictionary* json = [LayaApp parseJSON:data];
    NSString *title = [json objectForKey:@"title"];
    //NSString *textToShare = [json objectForKey:@"desc"];
    UIImage *imageToShare = [LayaApp getImageFromURL: [json objectForKey: @"imgsrc"]];
    NSURL *urlToShare = [NSURL URLWithString:[json objectForKey:@"link"]];
    NSArray *activityItems = @[title, imageToShare, urlToShare];
    UIActivityViewController *activityVC = [[UIActivityViewController alloc]initWithActivityItems:activityItems applicationActivities:nil];
    // 设置不出现在活动的项目
    activityVC.excludedActivityTypes =
    @[UIActivityTypePrint,UIActivityTypeMessage,UIActivityTypeMail,
    UIActivityTypePrint,UIActivityTypeAddToReadingList,UIActivityTypeOpenInIBooks,
    UIActivityTypeCopyToPasteboard,UIActivityTypeAssignToContact,UIActivityTypeSaveToCameraRoll];
    
    [[ViewController GetIOSViewController] presentViewController:activityVC animated:YES completion:nil];
     // 分享之后的回调
    activityVC.completionWithItemsHandler = ^(UIActivityType  _Nullable activityType, BOOL completed, NSArray * _Nullable returnedItems, NSError * _Nullable activityError) {
        if (completed) {
            NSLog(@"completed");
            NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity: 2];
            [dic setObject: @0 forKey:@"errCode"];
            [dic setObject: @"" forKey:@"errStr"];
            [LayaApp callback:@"share:data:" data: dic];
        } else  {
            NSLog(@"cancled");
            NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity: 2];
            [dic setObject: @-1 forKey:@"errCode"];
            [dic setObject: @"error " forKey:@"errStr"];
            [LayaApp callback:@"share:data:" data: dic];
        }
    };
}
+(UIImage *) getImageFromURL:(NSString *)fileURL {
 
    UIImage * result;
 
    NSData * data = [NSData dataWithContentsOfURL:[NSURL URLWithString:fileURL]];
 
    result = [UIImage imageWithData:data];
 
    return result;
 
}
//zh en
+(void)getLanguage {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSArray* languages = [defaults objectForKey:@"AppleLanguages"];
    NSString* currentLanguage = [languages objectAtIndex:0];
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString* languageCode = [temp objectForKey:NSLocaleLanguageCode];
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys: languageCode, @"language", nil];
    [LayaApp callback:@"getLanguage" data:dict];
}
+(void)switchLanguage:(NSString*) language {
    
    NSString* userLanguage = nil;
    if ([language isEqualToString:@"Chinese"])
    {
        userLanguage = @"zh-Hans";
    }
    else if ([language isEqualToString:@"English"]) {
        userLanguage = @"en";
    }
    else {
        userLanguage = @"zh-Hans";
    }
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setValue:@[userLanguage] forKey:@"AppleLanguages"];
    [defaults synchronize];
}
+(void)getAgeGateDecision:(NSString*)name birthDay:(NSString*)birthDay {
    //[[GCLayaAccountManager sharedInstance] getAgeGateDecision:birthDay success:^{
               //获取验证码成功
               NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithInt:0], @"errCode"
                                        , nil];
               [LayaApp callback:@"getAgeGateDecision:birthDay:" data:dict];
           //} failure:^(NSInteger errcode, NSString * _Nonnull errMsg) {
               //error.description
               //NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
               //                         [NSNumber numberWithInt:-1], @"errCode"
                //                     , [NSNumber numberWithInteger:errcode], @"errCodeSDK"
                //                     , errMsg, @"errMsg"
               //                         , nil];
               //[LayaApp callback:@"getAgeGateDecision:birthDay:" data:dict];
    //}];
}
+(NSString*) getModel
{
    size_t size;
    sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
    char *model = (char *)malloc(size);
    if (model == nullptr) {
        return @"";
    }
    sysctlbyname("hw.machine", model, &size, nullptr, 0);
    NSString *platform = [NSString stringWithCString:model encoding:NSUTF8StringEncoding];
    free(model);
    return platform;
    
    //NSString* nsDeviceModel =[[UIDevice currentDevice] model];
    //return [nsDeviceModel cStringUsingEncoding:NSUTF8StringEncoding];
}
+(void)getBenchmarkLevel:(NSString*)name url:(NSString*)url {
    NSString* fullUrl = [NSString stringWithFormat:@"%@/momoyo/api/bench?device_platform=ios&device_type=%@", url, [self getModel]];
    NSLog(@"getBenchmarkLevel:%@", fullUrl);
    //NSMutableDictionary *params = [NSMutableDictionary dictionary];
    //[params setValue:@"937ff5d690c1d2fc47a9b09f849424b3c50978ce" forKey:@"code"];
    //[params setValue:birthday forKey:@"birthday"];
    //[params setValue:@(2) forKey:@"sp_id"];//国内
    //[params setValue:@(3) forKey:@"sp_id"];//海外
    //[params setValue:@"laya_0012" forKey:@"game_id"];
    
    //NSDictionary *header = @{@"Content-Type" : @"application/x-www-form-urlencoded"};
    [GCLayaTTNetManager requestURLString: fullUrl
                                  params:nil
                                  method:@"GET"
                        needCommonParams:YES
                             headerField:nil success:^(NSDictionary * _Nonnull JSONObj, NSDictionary * _Nonnull responseHeaderFields) {
        NSLog(@"请求成功 JSONObj:%@ responseHeaderFields:%@", JSONObj, responseHeaderFields);
        NSNumber * code = [JSONObj valueForKey:@"code"];
        if (code.intValue == 0) {
            NSDictionary * data = [JSONObj valueForKey:@"data"];
            NSLog(@"getBenchmarkLevel ok %@", [data valueForKey:@"benchmark_level_str"]);
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInt:0], @"errCode"
                                  , [data valueForKey:@"benchmark_level_str"], @"benchmark_level_str"
                                  , nil];
            [LayaApp callback:@"getBenchmarkLevel:url:" data:dict];
        }
        else {
            NSLog(@"getBenchmarkLevel failed %@", [JSONObj valueForKey:@"message"]);
            NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithInt:-1], @"errCode"
                                  //, [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                                  , [JSONObj valueForKey:@"message"], @"errMsg"
                                     , nil];
            [LayaApp callback:@"getBenchmarkLevel:url:" data:dict];
        }
    } failure:^(NSError * _Nonnull error, NSDictionary * _Nullable responseHeaderFields) {
        NSLog(@"请求失败 error:%@ responseHeaderFields:%@", error, responseHeaderFields);
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithInt:-1], @"errCode"
                              //, [NSNumber numberWithInteger:error.code], @"errCodeSDK"
                              , [error localizedDescription], @"errMsg"
                                 , nil];
        [LayaApp callback:@"getBenchmarkLevel:url:" data:dict];
    }];
}
@end
