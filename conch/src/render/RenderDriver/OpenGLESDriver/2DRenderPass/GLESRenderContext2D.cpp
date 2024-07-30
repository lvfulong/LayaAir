#include "GLESRenderContext2D.h"
#include "render/LayaGL.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h"
#include "GLESRenderElement2D.h"

namespace laya
{
	GLESRenderElement2D* GLESRenderContext2D::blitscreenElement2D = nullptr;
	GLESRenderContext2D::GLESRenderContext2D() {

	}
	GLESRenderContext2D::~GLESRenderContext2D() {

	}
	void GLESRenderContext2D::setRenderTarget(GLESInternalRT* renderRT, bool clear, const Color &clearColor)
	{
		destRT = renderRT;
		if (destRT ) {
			LayaGL::m_pWebglEngine->getTextureContext()->bindRenderTarget(destRT, 0);
			GLESInternalTex* tex = destRT->m_textures.front();
			LayaGL::m_pWebglEngine->viewport(0, 0, tex->m_width, tex->m_height);
		}
		else
		{
			LayaGL::m_pWebglEngine->getTextureContext()->bindoutScreenTarget();
			LayaGL::m_pWebglEngine->viewport(0, 0, _offscreenWidth, _offscreenHeight);
		}

		LayaGL::m_pWebglEngine->scissorTest(false);
		uint32_t clearflag = clear ? static_cast<uint32_t>(RenderClearFlag::Color) : static_cast<uint32_t>(RenderClearFlag::Nothing);
		LayaGL::m_pWebglEngine->clearRenderTexture(clearflag, &clearColor, 1);
	}

	void GLESRenderContext2D::setOffscreenView(uint32_t width, uint32_t height)
	{

		_offscreenWidth = width;
		_offscreenHeight = height;
	}

	void GLESRenderContext2D::drawRenderElementOne(GLESRenderElement2D* node)
	{
		node->_preUpdatePre(this);
		node->_render(this);
	}

	void GLESRenderContext2D::setBlitScreenElement(GLESRenderElement2D* node)
	{
		GLESRenderContext2D::blitscreenElement2D = node;
	}



	void GLESRenderContext2D::drawRenderElementList(const JCSingletonList<GLESRenderElement2D*>& list)
	{
		for (int i  = 0, n  = list.getLength(); i < n; i++) {
			GLESRenderElement2D* element = list.m_vElements[i];
			element->_preUpdatePre(this);//render
		}
		for (int i = 0, n = list.getLength(); i < n; i++) {
			GLESRenderElement2D* element = list.m_vElements[i];
			element->_render(this);//render
		}
		return ;
	}
} // namespace laya
