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
#import "BDHub.h"

@implementation LayaApp

+(void)getSupports: (NSString*)function {
    NSMutableArray* supports = [[NSMutableArray alloc]init];
    if (WXApi.isWXAppInstalled){
        [supports addObject:@"wechat"];
    }
    
    //if ([TencentOAuth iphoneQQInstalled] || [TencentOAuth iphoneTIMInstalled]){
    //    [supports addObject:@"QQ"];
    //}
    [LayaApp callback:@"getSupports:" data:supports];
}

+(void)loginWX{
    SendAuthReq* req = [[SendAuthReq alloc]init];
    req.scope = @"snsapi_userinfo";
    
    [WXApi sendReq:req completion:nil];
}

+(void)loginQQ{
}
+(void)login:(NSString*)type {
    if ([type isEqualToString:@"wechat"]) {
        SendAuthReq* req = [[SendAuthReq alloc]init];
        req.scope = @"snsapi_userinfo";
        
        [WXApi sendReq:req completion:nil];
    }
    else if ([type isEqualToString:@"qq"]) {
        TencentOAuth * oauth = [AppDelegate TencentOAuth];
        oauth.authMode = kAuthModeClientSideToken;
        [oauth authorize:[LayaApp getPermissions]];
    }
}
+(void)logout:(NSString*)type {
    if ([type isEqualToString:@"wechat"]) {
        [LayaApp callback:@"logout:" data: [[NSDictionary alloc] init]];
    }
    else if ([type isEqualToString:@"qq"]) {
        TencentOAuth * oauth = [AppDelegate TencentOAuth];
        [oauth logout:[AppDelegate Instance]];
    }
}

+(void)share:(NSString*)type data:(NSString*)data{
    NSDictionary* json = [LayaApp parseJSON:data];
    [LayaApp _shareToMiniProgram:json];
}

+(void)_shareToMiniProgram:(NSDictionary*)json{
    WXMiniProgramObject* object = [WXMiniProgramObject object];
    object.webpageUrl = [json objectForKey:@"link"];
    object.miniProgramType = WXMiniProgramTypeRelease;
    object.userName = [json objectForKey:@"origin_id"];
    object.path = [json objectForKey:@"share_param"];
    object.withShareTicket = TRUE;
    object.hdImageData = [LayaApp getThumb:[json objectForKey:@"imgsrc"] maxFileSize:131072];
    //NSLog(@"image size:%lu",  (unsigned long)(object.hdImageData.length));
    if (object.hdImageData.length >= 131072){
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithInt:-2], @"errCode",
                              @"image size too large", @"errStr"
                              , nil];
        
        return [LayaApp callback:@"share:" data:dict];
    }
    WXMediaMessage* message = [WXMediaMessage message];
    message.title = [json objectForKey:@"title"];
    message.description = [json objectForKey:@"desc"];
    message.thumbData = nil;
    message.mediaObject = object;
    
    SendMessageToWXReq* req = [[SendMessageToWXReq alloc]init];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req completion:nil];
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

+(NSData*)getThumb:(NSString*)imageUrl maxFileSize:(CGFloat) maxFileSize{
    NSURL* url = [NSURL URLWithString:imageUrl];
    NSData* imageData = [[NSData alloc]initWithContentsOfURL:url];
    if (maxFileSize<=0){
        return imageData;
    }else{
        UIImage* image = [UIImage imageWithData:imageData];
        CGFloat quality = 0.9f;
        CGFloat minQuality = 0.5f;
        NSData* compressedData = UIImageJPEGRepresentation(image, quality);
        while ([compressedData length] >= maxFileSize && quality > minQuality){
            quality -= 0.1f;
            compressedData = UIImageJPEGRepresentation(image, quality);
        }
        return compressedData;
    }
}


+(void)checkIsAppInstalled:(NSString*)appType {
    if ([appType isEqualToString:@"wechat"]) {
        bool bIsWXAppInstalled = WXApi.isWXAppInstalled;
    
        NSMutableDictionary *dic=[NSMutableDictionary dictionaryWithCapacity:3];
        if (bIsWXAppInstalled)
        {
            [dic setObject: @TRUE forKey:@"isInstalled"];
            [dic setObject: @"" forKey:@"text"];
        }
        else
        {
            [dic setObject: @FALSE forKey:@"isInstalled"];
            [dic setObject: @"您尚未安装微信，请先安装。" forKey:@"text"];
        }
        [dic setObject: @"WX" forKey:@"loginType"];
        [LayaApp callback:@"checkIsAppInstalled:" data:dic];
    }
}
+(void)exit {
    abort();
}
+(NSMutableArray *)getPermissions
{
    NSMutableArray * g_permissions = [[NSMutableArray alloc] initWithObjects:kOPEN_PERMISSION_GET_USER_INFO,
                                      kOPEN_PERMISSION_GET_SIMPLE_USER_INFO,
                                      kOPEN_PERMISSION_ADD_ALBUM,
                                      kOPEN_PERMISSION_ADD_TOPIC,
                                      kOPEN_PERMISSION_CHECK_PAGE_FANS,
                                      kOPEN_PERMISSION_GET_INFO,
                                      kOPEN_PERMISSION_GET_OTHER_INFO,
                                      kOPEN_PERMISSION_LIST_ALBUM,
                                      kOPEN_PERMISSION_UPLOAD_PIC,
                                      kOPEN_PERMISSION_GET_VIP_INFO,
                                      kOPEN_PERMISSION_GET_VIP_RICH_INFO,
                                      nil];
    
    return g_permissions;
}
+(void)showRewardVideo:(NSString*) type data: (NSString*)strData{
    [BDHub.instance showRewardVideo:type data: strData];
}
@end
