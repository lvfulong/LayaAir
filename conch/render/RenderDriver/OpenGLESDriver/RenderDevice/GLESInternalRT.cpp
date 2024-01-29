#include "GLESInternalRT.h"
#include "GLESInternalTex.h"
namespace laya
{
	WebGLInternalRT::WebGLInternalRT(GLESEngine* engine, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool isCube, bool generateMipmap, int samples): GLObject(engine)
	{
		m_colorFormat = colorFormat;
		m_depthStencilFormat = depthStencilFormat;
		m_isCube = isCube;
		m_generateMipmap = generateMipmap;
		m_samples = samples;

		m_textures.clear();
		m_depthTexture.reset();

		glGenFramebuffers(1, &m_framebuffer);
		if (samples > 1)
		{
			glGenFramebuffers(1, &m_msaaFramebuffer);
		}
	}
	int WebGLInternalRT::getGpuMemory()
	{
		return m_gpuMemory;
	}
	void WebGLInternalRT::setGpuMemory(int value)
	{
		m_gpuMemory = value;
		m_engine->_addStatisticsInfo(RenderStatisticsInfo::GPUMemory, m_gpuMemory);
		m_engine->_addStatisticsInfo(RenderStatisticsInfo::RenderTextureMemory, m_gpuMemory);
	}

	WebGLInternalRT::~WebGLInternalRT()
	{
		dispose();
	}

	void WebGLInternalRT::dispose()
	{
		for (TexturesVec::iterator it = m_textures.begin(); it != m_textures.end(); it++)
		{
            it->reset();
		}
		m_textures.clear();
        
        m_depthTexture.reset();
	
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

}
