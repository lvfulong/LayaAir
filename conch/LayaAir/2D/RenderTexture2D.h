#ifndef __RenderTexture2D_H__
#define __RenderTexture2D_H__

#include <utils/Preprocessor.h>
#include <stdint.h> 
#include <stack> 
#include <render/driver/gles/WebGLInternalTex.h>
#include <render/driver/gles/WebGLInternalRT.h>
#include <memory>

namespace laya
{
	class RenderTexture2D;
	struct RenderTexture2DStackEntry
	{
		RenderTexture2D* rt;
		int w;
		int h;
		RenderTexture2DStackEntry(RenderTexture2D* rt2d, int width, int height): rt(rt2d),
			w(width),
			h(height)
		{

		}
	};
	class JCImage;
	class WebGLInternalRT;
	class RenderTexture2D
	{
	public:
		RenderTexture2D(WebGLEngine* pWebGLEngine, int width, int height, RenderTargetFormat format, RenderTargetFormat depthStencilFormat);
		~RenderTexture2D();
		void _create(int width, int height);
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		void start();
		void clear(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
		void end();
		void restore();
		int getID() { return m_texture->getID(); }
		static void pushRT();
		static void popRT();
		GLuint getGLFramebuffer();
	public:
		int                                                 m_width;
		int                                                 m_height;
		RenderTargetFormat                                  m_colorFormat;
		RenderTargetFormat                                  m_depthStencilFormat;
		std::shared_ptr<WebGLInternalTex>                   m_texture;
		WebGLInternalRT*                                    m_renderTarget;
		static RenderTexture2D*                             m_currentActive;
		RenderTexture2D*                                    m_lastRT = nullptr;
		int                                                 m_lastWidth = 0;
		int                                                 m_lastHeight = 0;
		static std::stack<RenderTexture2DStackEntry>        m_rtStack;
		WebGLEngine*                                        m_pWebGLEngine;
	};
}
#endif //__RenderTexture2D_H__
