#ifndef __BACKEND_H__
#define __BACKEND_H__

namespace laya {

struct BackendOptions {
    void* nativeContext = nullptr;//for iOS
    void* nativeLayer = nullptr;//for iOS
    bool alpha = true;
    bool depth = true;
    bool stencil = true;
    bool antialiasing = false;
    bool preserveBackBuffer = true;
};
} // namespace laya
#endif
