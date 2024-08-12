#ifndef __IBACKEND_H__
#define __IBACKEND_H__

namespace laya
{

struct BackendOptions
{
    void *nativeContext = nullptr; // for iOS
    void *nativeLayer = nullptr;   // for iOS
    bool alpha = true;
    bool depth = true;
    bool stencil = true;
    bool antialiasing = false;
    bool preserveBackBuffer = true;
};

class IBackend
{
  public:
    IBackend()
    {
    }
    virtual ~IBackend()
    {
    }
    virtual void create(const BackendOptions &options) = 0;
    virtual void swapBuffer() = 0;
    virtual void createScreenSurface(void *nativeHandle) = 0;
    virtual void makeCurrent() = 0;
    virtual void onScreenSurfaceResize(int width, int height) = 0;
    virtual void destroyScreenSurface() = 0;
};
IBackend *createBackend(const BackendOptions &options);
} // namespace laya
#endif
