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
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include "render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h"
#include <render/Property.h>
#if defined(OS_IOS)
#include "OpenGLBackendiOS.h"
#elif defined(OS_LINUX)
#include "OpenGLBackendLinuxEGLX11.h"
#elif defined(OS_ANDROID)
#include "OpenGLBackendAndroidEGL.h"
#elif defined(OS_WINDOWS)
#include "OpenGLBackendWinEGL.h"
#elif defined(OS_OHOS)
#include "OpenGLBackendOHOSEGL.h"
#endif
#include "render/LayaGL.h"
extern int g_nInnerHeight;
extern int g_nInnerWidth;

namespace laya
{
    extern int g_nMainFrameBuffer;
    extern int g_nRealMainFrameBuffer;
    extern GLESInternalRT* g_target;

	JCConchRender::JCConchRender(void* pFileResManager)
	{
        m_pRenderThread = NULL;
        m_nFrameCount = 0;
		m_fShowPerfScale = 0;
        m_pFileResManager = (JCFileResManager*)pFileResManager;
        m_blitContext = new GLESRenderContext2D();
        m_blitContext->pipelineMode = "Forward";
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
            //if (m_pScreenContext)
            //{
                //delete m_pScreenContext;
                //m_pScreenContext = 0;
            //}
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
        //if (m_pScreenContext) {
            //m_pScreenContext->size(width, height);
        //}
    });
}
void JCConchRender::start()
{
    if (!LayaGL::m_pWebglEngine)
    {
        return;
    }
    //webgl mode need restore gl state
    //if (m_pScreenContext == nullptr)
    //{
       // m_pScreenContext = new ScreenCanvasContext2D(LayaGL::m_pWebglEngine);
    //}
    //m_pMainContext->m_target->start();
    //m_pScreenContext->startForMainCanvas();
    //m_pMainContext->m_target->clear(0.0f, 0.0f, 0.0f, 1.0f);
}
void JCConchRender::end()
{
    if (!LayaGL::m_pWebglEngine)
    {
        return;
    }



    int last_width;
    int last_height;
    int width, height;
    int last_main_frame_buffer = g_nMainFrameBuffer;
    g_nMainFrameBuffer = g_nRealMainFrameBuffer;
    m_GfxBackend->getScreenSurfaceSize(&width, &height);
    m_blitContext->setOffscreenView(width, height);
    m_blitContext->setRenderTarget(nullptr, false, Color::BLACK);
    GLESRenderContext2D::blitscreenElement2D->materialShaderData->setInternalTexture(CommandProperty::SCREENTEXTURE_ID, g_target->m_textures[0]);
    m_blitContext->drawRenderElementOne(GLESRenderContext2D::blitscreenElement2D);
    g_nMainFrameBuffer = last_main_frame_buffer;
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
   // m_pScreenContext->requestCaptureScreen();
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
 #if defined(OS_IOS)
                m_GfxBackend = new OpenGLBackendiOS();
#elif defined(OS_LINUX)
                m_GfxBackend = new OpenGLBackendLinuxEGLX11();
#elif defined(OS_ANDROID)
                m_GfxBackend = new OpenGLBackendAndroidEGL();
#elif defined(OS_WINDOWS)
                m_GfxBackend = new OpenGLBackendWinEGL();
#elif defined(OS_OHOS)
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
