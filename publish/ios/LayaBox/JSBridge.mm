#import "JSBridge.h"
#import "AppDelegate.h"
@implementation JSBridge

+(void)hideSplash
{
        NSLog(@"hideSplash11");
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView hide];
}
+(void)showTips:(NSString*)type
{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView showTips:type];
}
+(void)setFontColor:(NSString*)color
{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setFontColor:color];
}
+(void)bgColor:(NSString*)color
{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setBackgroundColor:color];
}
+(void)loading:(NSNumber*)percent
{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setPercent:percent.integerValue];
}
+(void)showTextInfo:(NSNumber*)show
{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView showTextInfo:show.intValue > 0];
}
@end

