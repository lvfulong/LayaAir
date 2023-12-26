#import <Foundation/NSObject.h>
#import "iap/IAPManager.h"

@interface LayaApp: NSObject

+(void)callback:(NSString*)method data:(NSObject*) data;
@end
