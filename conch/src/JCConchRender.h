#ifndef __JCConchRender_H__
#define __JCConchRender_H__

#include "JCSystemConfig.h"
#include <LayaGL/JCLayaGL.h>
#include <WebGL/WebGLThread.h>
#include <atomic>
#include <backend/IBackend.h>
#include <render/3D/temp/ObjectBase.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <resource/JCFileResManager.h>
#include <utils/thread/JCWorkSemaphore.h>
#include <utils/thread/JCWorkerThread.h>

namespace laya
{
class ScreenCanvasContext2D;
class OpenGLBackendiOS;
class OpenGLBackendAndroidEGL;
class OpenGLBackendWinEGL;
class OpenGLBackendLinuxEGLX11;
class OpenGLBackendOHOSEGL;
class GLESRenderContext2D;
class JCConchRender
{
  public:
    JCConchRender(void *pFileResManager);
    ~JCConchRender();
    void init();
    void update();
    void clearAllData();
    void start();
    void end();
    void swapBuffer();
    void setMainContextSize(int width, int height);
    void requestCaptureScreen();
    void onScreenSurfaceResize(int width, int height);
    void createBackend(const BackendOptions &options);
    void createScreenSurface(void *nativeHandle);
    void destroyScreenSurface();

    template <typename F, typename... Args>
    auto postTaskFromJSToRenderSync(F &&f, Args &&...args) -> std::future<std::result_of_t<F(Args...)>>
    {
        #if 0
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
        {
            return m_WebGLThread->postTaskSync(f); // TODOs
        }
        else
        #endif
        {
            std::promise<std::result_of_t<F(Args...)>> p;
            p.set_value(f());
            return p.get_future();
        }
    }
    void postTaskFromJSToRenderAsync(std::function<void()> task)
    {
        #if 0
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
        {
            m_WebGLThread->postTaskAsync(task); // TODOs
        }
        else
        #endif
        {
            task();
        }
    }

  public:
    JCWorkerThread *m_pRenderThread;
    uint64_t m_nFrameCount = {0};
    JCImageManager *m_pImageManager;
    JCIDGenerator *m_pIDGenerator;
    JCIDGenerator *m_pProgramLocationTable;
    JCLayaGL *m_pLayaGL;
    JCFileResManager *m_pFileResManager;
    ObjectManager<GLESInternalTex> *m_pWebGLInternalTexManager;
    ObjectManager<UniformBufferObject> *m_pUniformBufferObjectManager;
    ScreenCanvasContext2D *m_pScreenContext = nullptr;
    GLESRenderContext2D *m_blitContext = nullptr;
    IBackend *m_GfxBackend = {nullptr};
    WebGLThread *m_WebGLThread = nullptr;
};
} // namespace laya
#endif