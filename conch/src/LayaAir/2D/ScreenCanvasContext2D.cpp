#include "ScreenCanvasContext2D.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCBuffer.h>
#include <algorithm>
#include <tuple> 
#include <array>
#include "JCConch.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <Bindings/JSLayaGL.h>
#include "BufferStateBase.h"
#include "JCSystemConfig.h"
#include <Bindings/JSInput.h>

extern int g_nInnerWidth;
extern int g_nInnerHeight;

namespace laya
{
	extern int g_nMainFrameBuffer;
	extern GLESInternalRT* g_target;
	static float INV_UV[8] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	//------------------------------------------------------------------------------
	ScreenCanvasContext2D::ScreenCanvasContext2D(GLESEngine* pGLESEngine)
	{
		m_pGLESEngine = pGLESEngine;
		m_width = g_nInnerWidth;
		m_height = g_nInnerHeight;
		g_target =m_target = m_pGLESEngine->getTextureContext()->createRenderTargetInternal(m_width, m_height, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None, false, false, 1);
		m_texture = m_target->m_textures[0];
	}
	//------------------------------------------------------------------------------
	ScreenCanvasContext2D::~ScreenCanvasContext2D()
	{
        if (m_target != nullptr)
        {
            delete m_target;
            m_target = nullptr;
        }
	}





	//------------------------------------------------------------------------------
	void ScreenCanvasContext2D::startForMainCanvas()
	{
		if (m_target)
		{
			m_pGLESEngine->getTextureContext()->bindRenderTarget(m_target, 0);
			g_nMainFrameBuffer = m_target->getGLFramebuffer();
		}
	}
	void ScreenCanvasContext2D::endForMainCanvas()
	{
		if (m_target)
		{
            if (m_requestCaptureScreen)
            {
                captureScreen();
                m_requestCaptureScreen = false;
            }
			m_pGLESEngine->getTextureContext()->unbindRenderTarget(m_target);
		}
	}

	void ScreenCanvasContext2D::size(int w, int h)
	{
		if (m_width != w || m_height != h) 
		{
			m_width = w;
			m_height = h;
			if (m_target != nullptr)
			{
				delete m_target;
				m_target = nullptr;
				g_target = m_pGLESEngine->getTextureContext()->createRenderTargetInternal(w, h, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None, false, false, 1);
				m_texture = g_target->m_textures[0];
				g_nMainFrameBuffer = g_target->getGLFramebuffer();
			}
		}
		if (w == 0 && h == 0)
		{
			//this._releaseMem();TODO
			LOGI("dd");
		}
	}
    void ScreenCanvasContext2D::requestCaptureScreen()
    {
        m_requestCaptureScreen = true;
    }
    void ScreenCanvasContext2D::captureScreen()
    {
        if (m_width > 0 && m_height > 0)
        {
            size_t w = m_width;
            size_t h = m_height;
            int len = m_width * m_height * 4;
            uint8_t* buffer = new uint8_t[len];
            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, w, h, buffer);
            JSInput::getInstance()->captureScreenCallBack((char*)buffer, len, (int)w, (int)h);
        }
    }
}

