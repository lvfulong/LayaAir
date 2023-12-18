//
//  LayaApp.h
//  WKWebview
//
//  Created by Alex on 2019/9/5.
//  Copyright © 2019 LayaBox. All rights reserved.
//

#import <Foundation/NSObject.h>
#import "iap/IAPManager.h"

@interface LayaApp: NSObject

+(void)callback:(NSString*)method data:(NSObject*) data;
@end
