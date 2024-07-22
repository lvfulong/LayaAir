#import "HandleMessageUtils.h"
@implementation HandleMessageUtils

+(NSString*)handleSyncMessageWithEventName:(NSString*)eventName data:(NSString*)data {
    NSLog(@"%@ %@", eventName, data);
    return @"sync result";
}
+(void)handleAsyncMessageWithEventName:(NSString*)eventName data:(NSString*)data callback:(void (^)(NSString *))cb {
    NSLog(@"%@ %@", eventName, data);
    cb(@"async result");
}
@end
