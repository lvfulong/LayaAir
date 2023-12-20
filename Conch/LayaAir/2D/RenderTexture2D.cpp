#include "RenderTexture2D.h"
#include <Utils/Log.h>
#include <render/3D/temp/RenderStateContext.h>
#include "RenderState2D.h"
#include <render/driver/gles/WebGLEngine.h>
#include <render/driver/gles/GLTextureContext.h>
#include <render/driver/gles/WebGLInternalRT.h>
namespace laya
{
	static Color _clearColor;
	static Color _clearLinearColor;
	static WebGLEngine* s_pWebGLEngine = nullptr;
	std::stack<RenderTexture2DStackEntry> RenderTexture2D::m_rtStack;
	RenderTexture2D* RenderTexture2D::m_currentActive = nullptr;
	RenderTexture2D::RenderTexture2D(WebGLEngine* pWebGLEngine, int width, int height, RenderTargetFormat format, RenderTargetFormat depthStencilFormat)
	{

		m_pWebGLEngine = s_pWebGLEngine = pWebGLEngine;
		m_width = width;
		m_height = height;
		m_colorFormat = format;
		m_depthStencilFormat = depthStencilFormat;
		_create(width, height);
	}
	RenderTexture2D::~RenderTexture2D()
	{
        if (m_renderTarget)
        {
            delete m_renderTarget;
            m_renderTarget = nullptr;
        }
	}
	void RenderTexture2D::_create(int width, int height)
	{
		m_renderTarget = m_pWebGLEngine->getTextureContext()->createRenderTargetInternal(width, height, m_colorFormat, m_depthStencilFormat, false, true, 1);
		m_texture = m_renderTarget->m_textures[0];
	}
	void RenderTexture2D::pushRT()
	{
		RenderTexture2D::m_rtStack.emplace(RenderTexture2D::m_currentActive, RenderState2D::width, RenderState2D::height);
	}
	void RenderTexture2D::popRT()
	{
		
		if (!RenderTexture2D::m_rtStack.empty()) 
		{
			RenderTexture2DStackEntry top = RenderTexture2D::m_rtStack.top();
			RenderTexture2D::m_rtStack.pop();
			if (RenderTexture2D::m_currentActive != top.rt) 
			{
				top.rt ? s_pWebGLEngine->getTextureContext()->bindRenderTarget(top.rt->m_renderTarget, 0) : s_pWebGLEngine->getTextureContext()->bindoutScreenTarget();
				RenderTexture2D::m_currentActive = top.rt;
			}
			s_pWebGLEngine->viewport(0, 0, top.w, top.h);
			RenderState2D::width = top.w;
			RenderState2D::height = top.h;
		}
	}
	void RenderTexture2D::start()
	{
		m_pWebGLEngine->getTextureContext()->bindRenderTarget(m_renderTarget, 0);
		m_lastRT = RenderTexture2D::m_currentActive;
		RenderTexture2D::m_currentActive = this;
		m_pWebGLEngine->viewport(0, 0, m_width, m_height);//�ⲿ����
		m_lastWidth = RenderState2D::width;
		m_lastHeight = RenderState2D::height;
		RenderState2D::width = m_width;
		RenderState2D::height = m_height;
		//BaseShader.activeShader = null;
	}
	void RenderTexture2D::clear(float r, float g, float b, float a)
	{

		_clearColor.r = r;
		_clearColor.g = g;
		_clearColor.b = b;
		_clearColor.a = a;
		_clearColor.toLinear(_clearLinearColor);
		m_pWebGLEngine->clearRenderTexture((uint32_t)RenderClearFlag::Color | (uint32_t)RenderClearFlag::Depth, &_clearLinearColor, 1.0f);
	}
	void RenderTexture2D::end()
	{
		m_pWebGLEngine->getTextureContext()->unbindRenderTarget(m_renderTarget);
		RenderTexture2D::m_currentActive = nullptr;
		//m_readyed = true;
	}
	void RenderTexture2D::restore()
	{
		if (m_lastRT != RenderTexture2D::m_currentActive)
		{
			if (m_lastRT) 
			{
				m_pWebGLEngine->getTextureContext()->bindRenderTarget(m_lastRT->m_renderTarget, 0);
			}
			else 
			{
				m_pWebGLEngine->getTextureContext()->unbindRenderTarget(m_renderTarget);
			}

			RenderTexture2D::m_currentActive = m_lastRT;
		}
		//m_readyed = true;

		m_pWebGLEngine->viewport(0, 0, m_lastWidth, m_lastHeight);
		RenderState2D::width = m_lastWidth;
		RenderState2D::height = m_lastHeight;
		//BaseShader.activeShader = null;
	}
	GLuint RenderTexture2D::getGLFramebuffer()
	{
		return m_renderTarget->getGLFramebuffer();
	}
	//------------------------------------------------------------------------------
}
