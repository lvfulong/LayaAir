#import "JSBridge.h"
#import "AppDelegate.h"
@implementation JSBridge

+(void)hideSplash
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView hide];
    });
}
+(void)showTips:(NSString*)type
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView showTips:type];
    });
}
+(void)setFontColor:(NSString*)color
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setFontColor:color];
    });
}
+(void)bgColor:(NSString*)color
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setBackgroundColor:color];
    });
}
+(void)loading:(NSNumber*)percent
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setPercent:percent.integerValue];
    });
}
+(void)showTextInfo:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView showTextInfo:show.intValue > 0];
    });
}
@end

