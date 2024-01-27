#ifndef __WebGLInternalRT_H__
#define __WebGLInternalRT_H__

#include <unordered_map>
#include <render/3D/design/renderEnum/RenderParams.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include <render/3D/design/renderEnum/RenderTargetFormat.h>
#include <vector>
#include <memory>
#include <utils/Preprocessor.h>

namespace laya
{
	class WebGLInternalTex;
	class WebGLEngine;
	class WebGLInternalRT : public GLObject
	{
	public:
		WebGLInternalRT(WebGLEngine* engine, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool isCube, bool generateMipmap, int samples);
		~WebGLInternalRT();
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
	public:
		GLuint										                m_framebuffer = 0;
		GLuint										                m_depthbuffer = 0;
		GLuint										                m_msaaFramebuffer = 0;
		GLuint										                m_msaaRenderbuffer = 0;
		bool										                m_isCube;
		int											                m_samples;
		bool										                m_generateMipmap;
		typedef std::vector<std::shared_ptr<WebGLInternalTex>>		TexturesVec;
        std::vector<std::shared_ptr<WebGLInternalTex>>				m_textures;
        std::shared_ptr<WebGLInternalTex>							m_depthTexture;
		RenderTargetFormat							                m_colorFormat;
		RenderTargetFormat							                m_depthStencilFormat;
		int											                m_gpuMemory = 0;
	};
}
#endif
