#import "JSBridge.h"
#import "AppDelegate.h"

#import "ViewController.h"
#import "UIFloatPanel.h"
#import "ScanViewController.h"
#import <UIKit/UINavigationController.h>
#import "NavViewController.h"

@implementation JSBridge

+(void)hideSplash
{
    NSLog(@"hideSplash11");
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
+(void)showScanner:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        if (show.intValue > 0)
        {
            [appDelegate.navigationController presentViewController:[[ScanViewController alloc] init] animated:NO completion:nil];
        }
        else
        {
            
        }
    });
}
+(void)onScanResult:(NSString*)result
{
    NSString* js = [[NSString alloc] initWithFormat:@"window.layaPlayerOnScanResult('%@')", result];
    [[conchRuntime GetIOSConchRuntime] runJS:js];
}
+(void)showFloatPanel:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (show.intValue > 0)
        {
            [[ViewController GetIOSViewController] showFloatPanel:false];
        }
        else
        {
            [[ViewController GetIOSViewController] showFloatPanel:true];
        }
    });
}
@end

