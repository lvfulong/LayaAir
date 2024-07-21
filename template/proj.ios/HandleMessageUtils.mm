#import "HandleMessageUtils.h"
#import "AppDelegate.h"
@implementation HandleMessageUtils

+(NSString*)handleSyncMessageWithEventName:(NSString*)eventName data:(NSString*)data {
    NSLog(@"%@ %@", eventName, data);
    return @"sync result";
}
@end
