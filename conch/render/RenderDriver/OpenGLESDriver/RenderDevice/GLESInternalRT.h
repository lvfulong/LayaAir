#ifndef __GLESInternalRT_H__
#define __GLESInternalRT_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include <memory>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <render/3D/design/renderEnum/RenderTargetFormat.h>
#include <unordered_map>
#include <utils/Preprocessor.h>
#include <vector>
#include <binder/JSInterface.h>
namespace laya
{
class WebGLInternalTex;
class GLESEngine;
class GLESInternalRT : public GLObject
{
  public:
    GLESInternalRT(RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool isCube,
                   bool generateMipmap, int samples);
    ~GLESInternalRT();
    void dispose();
    GLuint getGLFramebuffer()
    {
        if (m_samples > 1)
        {
            return m_msaaFramebuffer;
        }
        return m_framebuffer;
    }
    int getGpuMemory();
    void setGpuMemory(int value);
    bool isCube()
    {
        return -m_isCube;
    }
    JsValue getDepthTexture();
    // TODO JsValue getTextures();
  public:
    GLuint m_framebuffer = 0;
    GLuint m_depthbuffer = 0;
    GLuint m_msaaFramebuffer = 0;
    GLuint m_msaaRenderbuffer = 0;
    bool m_isCube;
    int m_samples;
    bool m_generateMipmap;
    typedef std::vector<std::shared_ptr<WebGLInternalTex>> TexturesVec;
    std::vector<std::shared_ptr<WebGLInternalTex>> m_textures;
    std::shared_ptr<WebGLInternalTex> m_depthTexture;
    RenderTargetFormat m_colorFormat;
    RenderTargetFormat m_depthStencilFormat;
    int m_gpuMemory = 0;
    bool _isSRGB = false;
    Persistent m_pJSTextures;
    Persistent m_pJSDepthTexture;
};
} // namespace laya
#endif
