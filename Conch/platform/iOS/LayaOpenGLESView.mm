#import <LayaOpenGLESView.h>
#include "JCConch.h"

@implementation LayaOpenGLESView {
}
@synthesize context;
- (instancetype)initWithFrame:(CGRect)frame contentScaleFactor:(CGFloat)scaleFactor {
    if (self = [super initWithFrame:frame]) {
        self.contentScaleFactor = scaleFactor;
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
    }
    
    return self;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
- (void)dealloc {
    self.context = nil;
}
@end
