/**
@file			JCConchRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCConchRender.h"
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include "JCSystemConfig.h"
#include "JCScriptRuntime.h"
#include <Bindings/JSConchConfig.h>
#include "JCConch.h"
#include <Bindings/JSLayaGL.h>
#include <LayaGL/JCLayaGLDispatch.h>
#include "../../LayaAir/2D/ScreenCanvasContext2D.h"
#include "../../LayaAir/2D/RenderTexture2D.h"
#include "LayaAir/2D/RenderState2D.h"
#include <render/3D/temp/RenderStateContext.h>
#ifdef OS_IOS
#include "OpenGLBackendiOS.h"
#elif OS_LINUX
#include "OpenGLBackendLinuxEGLX11.h"
#elif OS_ANDROID
#include "OpenGLBackendAndroidEGL.h"
#elif OS_WINDOWS
#include "OpenGLBackendWinEGL.h"
#elif OS_OHOS
#include "OpenGLBackendOHOSEGL.h"
#endif
#include "render/LayaGL.h"
extern int g_nInnerHeight;
extern int g_nInnerWidth;

namespace laya
{
    extern int g_nMainFrameBuffer;
    extern int g_nRealMainFrameBuffer;

	JCConchRender::JCConchRender(void* pFileResManager)
	{
        m_pRenderThread = NULL;
        m_nFrameCount = 0;
		m_fShowPerfScale = 0;
        m_pFileResManager = (JCFileResManager*)pFileResManager;
	}
    void JCConchRender::init()
    {
        m_pImageManager = new JCImageManager();
        m_pIDGenerator = new JCIDGenerator();
        m_pProgramLocationTable = new JCIDGenerator();
        m_pIDGenerator->reset();
        m_pProgramLocationTable->reset();
        m_pLayaGL = new JCLayaGL(g_nInnerWidth, g_nInnerHeight,m_pImageManager, m_pIDGenerator, m_pProgramLocationTable);
        JCLayaGLDispatch::ms_pLayaGL = m_pLayaGL;
        

        m_pWebGLInternalTexManager = new ObjectManager<GLESInternalTex>();
        //m_pShaderDataManager = new ResourceManager<ShaderData>();
        //m_pShaderInstanceManager = new ResourceManager<ShaderInstance>();
        //m_pRenderGeometryElementManager = new ResourceManager<RenderGeometryElement>();
        //m_pWordTextManager = new ObjectManager<WordText>();
        m_pUniformBufferObjectManager = new ObjectManager<UniformBufferObject>();
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) { 
            m_WebGLThread = new WebGLThread();
        }
    }
	JCConchRender::~JCConchRender()
	{
        //这个是在JCConch中传过来的，但是为了确保最后一帧，在这释放了，因为JCConchRender全局只有一个
        if (m_pFileResManager)
        {
            //这个fileCache也是同理，需要在这删除，
            if (m_pFileResManager->m_pFileCache)
            {
                delete m_pFileResManager->m_pFileCache;
                m_pFileResManager->m_pFileCache = NULL;
            }
            delete m_pFileResManager;
            m_pFileResManager = NULL;
        }
        if (m_pImageManager)
        {
            delete m_pImageManager;
            m_pImageManager = NULL;
        }
        if (m_pLayaGL)
        {
            delete m_pLayaGL;
            m_pLayaGL = NULL;
        }

        if (m_pIDGenerator)
        {
            delete m_pIDGenerator;
            m_pIDGenerator = NULL;
        }
        if (m_pProgramLocationTable)
        {
            delete m_pProgramLocationTable;
            m_pProgramLocationTable = NULL;
        }
        
         auto  func = [this]()->bool {
            if (m_pScreenContext)
            {
                delete m_pScreenContext;
                m_pScreenContext = 0;
            }
             /*if (m_pWordTextManager)
            {
                delete m_pWordTextManager;
                m_pWordTextManager = NULL;
            }
           if (m_pRenderGeometryElementManager)
            {
                delete m_pRenderGeometryElementManager;
                m_pRenderGeometryElementManager = NULL;
            }
            if (m_pShaderInstanceManager)
            {
                delete m_pShaderInstanceManager;
                m_pShaderInstanceManager = NULL;
            }
            if (m_pShaderDataManager)
            {
                delete m_pShaderDataManager;
                m_pShaderDataManager = NULL;
            }*/
            if (m_pWebGLInternalTexManager)
            {
                delete m_pWebGLInternalTexManager;
                m_pWebGLInternalTexManager = NULL;
            }
            if (m_pUniformBufferObjectManager)
            {
                delete m_pUniformBufferObjectManager;
                m_pUniformBufferObjectManager = NULL;
            }
            if (LayaGL::m_pWebglEngine)
            {   
                delete LayaGL::m_pWebglEngine;
                LayaGL::m_pWebglEngine = nullptr;
            }

            delete m_GfxBackend;
            m_GfxBackend = nullptr;
            return true;
        };
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
            m_WebGLThread->postTaskSync(func).get();
        }
        else {
            JCConch::s_pScriptRuntime->m_pScriptThread->postTaskSync(func).get();
        }

        if (m_WebGLThread != nullptr)
        {
            delete m_WebGLThread;
            m_WebGLThread = nullptr;
        }
	}
    void JCConchRender::update() {
        m_nFrameCount++;
        if (m_nFrameCount % 60 == 0)
        {
            m_pImageManager->update(m_nFrameCount);
        }
    }
	void JCConchRender::clearAllData()
	{
        LOGI(">>>JCConchRender::clearAllData = %s", ToString<std::thread::id >::convert(std::this_thread::get_id()).c_str());
        //m_kPerfRender.invalidGLRes();
        m_pLayaGL->deleteAllGLRes();
        //图片全部清空
        if (m_pImageManager) {
            m_pImageManager->resetRenderThread();
        }
        m_pIDGenerator->reset();
        m_pProgramLocationTable->reset();
	}

void JCConchRender::setMainContextSize(int width,int height)
{
    postTaskFromJSToRenderAsync([width, height, this]() {
        JCConch::s_pConchRender->m_pLayaGL->m_nMainCanvasWidth = width;
        JCConch::s_pConchRender->m_pLayaGL->m_nMainCanvasHeight = height;
        if (m_pScreenContext) {
            m_pScreenContext->size(width, height);
        }
    });
}
void JCConchRender::start()
{
    if (!LayaGL::m_pWebglEngine)
    {
        return;
    }
    //webgl mode need restore gl state
    if (m_pScreenContext == nullptr)
    {
        m_pScreenContext = new ScreenCanvasContext2D(LayaGL::m_pWebglEngine);
    }
    //m_pMainContext->m_target->start();
    m_pScreenContext->startForMainCanvas();
    //m_pMainContext->m_target->clear(0.0f, 0.0f, 0.0f, 1.0f);
}
void JCConchRender::end()
{
    if (!LayaGL::m_pWebglEngine)
    {
        return;
    }
    GLenum last_active_texture;
    GLint last_program;
    GLint last_texture;
    GLint last_sampler;
    GLint last_array_buffer;
    GLint last_element_array_buffer;
    GLint last_vertex_array;
    GLint last_polygon_mode[2];
    GLint last_viewport[4];
    GLint last_scissor_box[4];
    GLenum last_blend_src_rgb;
    GLenum last_blend_dst_rgb;
    GLenum last_blend_src_alpha;
    GLenum last_blend_dst_alpha;
    GLenum last_blend_equation_rgb;
    GLenum last_blend_equation_alpha;
    GLboolean last_enable_blend;
    GLboolean last_enable_cull_face;
    GLboolean last_enable_depth_test;
    GLboolean last_enable_scissor_test;
    int last_width;
    int last_height;
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
    {
        glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
        glActiveTexture(GL_TEXTURE0);
        glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
        //glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
        glGetIntegerv(GL_VIEWPORT, last_viewport);
        glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
        glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
        glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
        glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
        glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
        glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
        last_enable_blend = glIsEnabled(GL_BLEND);
        last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
        last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
        last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    }
    last_width = RenderState2D::width;
    last_height = RenderState2D::height;
    int last_main_frame_buffer = g_nMainFrameBuffer;
    g_nMainFrameBuffer = g_nRealMainFrameBuffer;
    m_pScreenContext->endForMainCanvas();
    //int width = m_pScreenContext->m_target->getWidth();
    //int height = m_pScreenContext->m_target->getHeight();
    Matrix m(JCLayaGL::s_fMainCanvasScaleX, 0.0f, 0.0f, JCLayaGL::s_fMainCanvasScaleY, JCLayaGL::s_fMainCanvasTX, JCLayaGL::s_fMainCanvasTY);
    static float INV_UV[8] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
    LayaGL::m_pWebglEngine->viewport(0, 0, g_nInnerWidth, g_nInnerHeight);
    LayaGL::m_pWebglEngine->scissor(0, 0, g_nInnerWidth, g_nInnerHeight);
    //glClearColor(1, 1, 1, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    RenderState2D::width = g_nInnerWidth;
    RenderState2D::height = g_nInnerHeight;
    m_pScreenContext->clear();
    //m_pScreenContext->size(g_nInnerWidth, g_nInnerHeight);
    //
    //临时在最好的渲染前恢复状态
    RenderStateContext::setDepthTest(false);
    RenderStateContext::setCullFace(false);
    RenderStateContext::setBlend(false);
    RenderStateContext::setDepthFunc(CompareFunction::Always);
    RenderStateContext::setStencilTest(false);
    m_pScreenContext->drawToScreen(m);
    //m_pScreenContext->drawTarget(m_pScreenContext->m_target, 0, 0, width, height, m, INV_UV, BlendMode::disable);
    m_pScreenContext->flush();
    m_pScreenContext->clear();
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
    {
        glUseProgram(last_program);
        glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindSampler(0, last_sampler);
        glActiveTexture(last_active_texture);
        glBindVertexArray(last_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
        glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
        glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
        if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
        if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
        if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
        LayaGL::m_pWebglEngine->viewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
        LayaGL::m_pWebglEngine->scissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
        
    }
    g_nMainFrameBuffer = last_main_frame_buffer;
    RenderState2D::width = last_width;
    RenderState2D::height = last_height;
}
void JCConchRender::swapBuffer()
{
    if (m_GfxBackend != nullptr)
    {
        m_GfxBackend->swapBuffer();
    }
}
void JCConchRender::requestCaptureScreen()
{
    m_pScreenContext->requestCaptureScreen();
}
    void JCConchRender::createScreenSurface(void *nativeHandle)
    {
        auto  func = [this, nativeHandle]() {
            m_GfxBackend->createScreenSurface(nativeHandle);
            m_GfxBackend->makeCurrent();
            if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL && LayaGL::m_pWebglEngine == nullptr)
            {
                WebGLConfig config;
                LayaGL::m_pWebglEngine = new GLESEngine(config, WebGLMode::Auto);
                LayaGL::m_pWebglEngine->initRenderEngine();
                LayaGL::m_pWebglEngine->createTextureContext(LayaGL::m_pWebglEngine->isWebGL2());
            }
        };
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
            m_WebGLThread->postTaskAsync(func);
        }
        else {
            JCConch::s_pScriptRuntime->m_pScriptThread->post(func);
        }
    }
    void JCConchRender::onScreenSurfaceResize(int width, int height)
    {
         auto  func = [this, width, height]() { 
            m_GfxBackend->onScreenSurfaceResize(width, height);
        };
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
             m_WebGLThread->postTaskAsync(func);
        }
        else {
            JCConch::s_pScriptRuntime->m_pScriptThread->post(func);
        }
    }
    void JCConchRender::destroyScreenSurface()
    {
        auto  func = [this]() {
            m_GfxBackend->destroyScreenSurface();
        };
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
            m_WebGLThread->postTaskAsync(func);
        }
        else {
            JCConch::s_pScriptRuntime->m_pScriptThread->post(func);
        }
    }
    void JCConchRender::createBackend(const BackendOptions& options)
    {
        auto  func = [this, options]() { 
                        if (m_GfxBackend == nullptr) {
 #ifdef OS_IOS
                m_GfxBackend = new OpenGLBackendiOS();
#elif OS_LINUX
                m_GfxBackend = new OpenGLBackendLinuxEGLX11();
#elif OS_ANDROID
                m_GfxBackend = new OpenGLBackendAndroidEGL();
#elif OS_WINDOWS
                m_GfxBackend = new OpenGLBackendWinEGL();
#elif OS_OHOS
                m_GfxBackend = new OpenGLBackendOHOSEGL();
#endif
                m_GfxBackend->create(options);
            }
        };
        if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
            m_WebGLThread->postTaskAsync(func);
        }
        else {
            JCConch::s_pScriptRuntime->m_pScriptThread->post(func);
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
