#include "GLESInternalRT.h"
#include "GLESInternalTex.h"
#include "render/driver/gles/LayaGL.h"

namespace laya
{
GLESInternalRT::GLESInternalRT(RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool isCube,
                               bool generateMipmap, int samples)
    : GLObject(LayaGL::m_pWebglEngine)
{
    m_colorFormat = colorFormat;
    m_depthStencilFormat = depthStencilFormat;
    m_isCube = isCube;
    m_generateMipmap = generateMipmap;
    m_samples = samples;

    m_textures.clear();

    glGenFramebuffers(1, &m_framebuffer);
    if (samples > 1)
    {
        glGenFramebuffers(1, &m_msaaFramebuffer);
    }
}
int GLESInternalRT::getGpuMemory()
{
    return m_gpuMemory;
}
void GLESInternalRT::setGpuMemory(int value)
{
    m_gpuMemory = value;
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::GPUMemory, m_gpuMemory);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::RenderTextureMemory, m_gpuMemory);
}

GLESInternalRT::~GLESInternalRT()
{

    m_pJSDepthTexture.reset();
    m_pJSTextures.reset();

    dispose();
}

void GLESInternalRT::dispose()
{
    for (TexturesVec::iterator it = m_textures.begin(); it != m_textures.end(); it++)
    {
        delete *it;
    }
    m_textures.clear();

    //if (m_depthTexture)
    //{
    //    delete m_depthTexture;
    //}

    if (m_framebuffer)
    {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
    if (m_depthbuffer)
    {
        glDeleteRenderbuffers(1, &m_depthbuffer);
        m_depthbuffer = 0;
    }
    if (m_msaaFramebuffer)
    {
        glDeleteFramebuffers(1, &m_msaaFramebuffer);
        m_msaaFramebuffer = 0;
    }
    if (m_msaaRenderbuffer)
    {
        glDeleteRenderbuffers(1, &m_msaaRenderbuffer);
        m_msaaRenderbuffer = 0;
    }

    m_engine->_addStatisticsInfo(RenderStatisticsInfo::GPUMemory, -m_gpuMemory);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::RenderTextureMemory, -m_gpuMemory);
    m_gpuMemory = 0;
}
JsValue GLESInternalRT::getDepthTexture()
{
    if (m_pJSDepthTexture.isEmpty())
    {
        //JSWebGLInternalTex *pTex = new JSWebGLInternalTex(m_pRenderTexture->m_depthTexture);
       // m_pJSDepthTexture.reset(JSP_TO_JS(JSWebGLInternalTex *, pTex));
        return m_pJSDepthTexture.toLocal().handle_;
    }
    else
    {
        return m_pJSDepthTexture.toLocal().handle_;
    }
}
JsValue GLESInternalRT::getTextures()
{
    if (m_pJSTextures.isEmpty())
    {
        int size = this->m_textures.size();
        std::vector<GLESInternalTex*> vec;
        vec.reserve(size);
        for (int i = 0; i < size; i++)
        {
            vec.push_back(this->m_textures[i]);
        }
        m_pJSTextures.reset(Converter<std::vector<GLESInternalTex*>>::ToJs(vec));
        return m_pJSTextures.toLocal().handle_;
    }
    else
    {
        return m_pJSTextures.toLocal().handle_;
    }
}
} // namespace laya
