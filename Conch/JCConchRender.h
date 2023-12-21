/**
@file			JCConchRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCConchRender_H__
#define __JCConchRender_H__


#include <resource/JCFileResManager.h>
#include <Utils/Thread/JCWorkSemaphore.h>
//#include <Performance/JCPerfHUD.h>
#include <LayaGL/JCLayaGL.h>
#include <Utils/Thread/JCWorkerThread.h>
#ifdef __APPLE__
#include "IOSFreetype/JCIOSFreeType.h"
#endif
#include <atomic>
#include <WebGL/WebGLThread.h>
#include <Backend.h>
#include <render/3D/temp/ObjectBase.h>
#include <render/3D/temp/ResourceBase.h>
#include <render/3D/temp/RenderGeometryElement.h>
#include <render/driver/gles/WebGLInternalTex.h>
#include <render/3D/temp/ShaderData.h>
#include "LayaAir/2D/WordText.h"
#include <render/3D/temp/UniformBufferObject.h>
#include <render/3D/temp/ShaderInstance.h>

namespace laya
{
    class ScreenCanvasContext2D;
    class JCFileSource;
    class OpenGLBackendiOS;
	class OpenGLBackendAndroidEGL;
    class OpenGLBackendWinEGL;
    class OpenGLBackendLinuxEGLX11;
    class JCConchRender
    {
    public:
        JCConchRender(void* pFileResManager);
        ~JCConchRender();
        void init();
        void update();
        void clearAllData();
        void start();
        void end();
        void swapBuffer();
        void setMainContextSize(int width,int height);
        void requestCaptureScreen();
        void onScreenSurfaceResize(int width, int height);
        void createBackend(const BackendOptions& options);
		void createScreenSurface(void *nativeHandle);
		void destroyScreenSurface();

        template <typename F, typename ... Args>
   	    auto postTaskFromJSToRenderSync(F&& f, Args&& ... args)->std::future<std::result_of_t<F(Args...)>> {
#ifdef WEBGL_THREAD
        return m_WebGLThread->postTaskSync(f);//TODOs
#else
        std::promise<std::result_of_t<F(Args...)>> p;
        p.set_value(f());
        return p.get_future();
#endif
    	}
        void postTaskFromJSToRenderAsync(std::function<void()> task) {
#ifdef WEBGL_THREAD
         m_WebGLThread->postTaskSync(task);//TODOs
#else
         task();
#endif
        }
    public:
        JCWorkerThread*                         m_pRenderThread;
        float				                    m_fShowPerfScale;               ///<是否显示性能测试
        uint64_t				                m_nFrameCount = { 0 };                  ///<frameCount
        JCImageManager*                         m_pImageManager;                ///<Image管理器
        JCIDGenerator*                          m_pIDGenerator;
        JCIDGenerator*                          m_pProgramLocationTable;
        JCLayaGL*                               m_pLayaGL;                      ///<layaGL
        JCFileResManager*                       m_pFileResManager;              ///<FileResManager 外部设置的
        //JCPerfDataRender                        m_kPerfRender;                  ///<性能测试
		ObjectManager<WebGLInternalTex>*		m_pWebGLInternalTexManager;
		ResourceManager<ShaderData>*			m_pShaderDataManager;
        ResourceManager<ShaderInstance>*		m_pShaderInstanceManager;
        ResourceManager<RenderGeometryElement>* m_pRenderGeometryElementManager;
		ObjectManager<WordText>*				m_pWordTextManager;
		ObjectManager<UniformBufferObject>*	    m_pUniformBufferObjectManager;
        ScreenCanvasContext2D*                  m_pScreenContext = nullptr;

#ifdef __APPLE__
        OpenGLBackendiOS*                       m_GfxBackend = { nullptr };
#elif LINUX
		OpenGLBackendLinuxEGLX11*                m_GfxBackend = { nullptr };
#elif ANDROID
		OpenGLBackendAndroidEGL*                m_GfxBackend = { nullptr };
#elif _WIN32
        OpenGLBackendWinEGL*   m_GfxBackend = { nullptr };
#endif
#ifdef WEBGL_THREAD
        WebGLThread*                            m_WebGLThread = nullptr;
#endif
    };
}
//------------------------------------------------------------------------------
#endif //__JCConchRender_H__

//-----------------------------END FILE--------------------------------
