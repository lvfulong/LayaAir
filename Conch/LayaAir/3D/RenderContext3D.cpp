#include "RenderContext3D.h"
#include "../RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"

namespace laya
{
	RenderContext3D::RenderContext3D(WebGLEngine* pEngine)
		:m_pEngine(pEngine)
	{
		m_pViewport = new Viewport(0, 0, 0, 0);
		m_pScissor = new Vector4();
		m_bInvertY = false;
		m_nSceneID = 0;
		m_nCameraUpdateMark=0;
		m_pCameraShaderData = NULL;
		m_pSceneShaderData = NULL;
		m_pDstTarget = NULL;
		m_pGlobalShaderData = NULL;
	}
	RenderContext3D::~RenderContext3D()
	{
		if (m_pViewport)
		{
			delete m_pViewport;
			m_pViewport = NULL;
		}
		if ( m_pScissor )
		{
			delete m_pScissor;
			m_pScissor = NULL;
		}
	}
	void RenderContext3D::applyContext(int cameraUpdateMark)
	{
		m_nCameraUpdateMark = cameraUpdateMark;
		m_pEngine->viewport(m_pViewport->x, m_pViewport->y, m_pViewport->width, m_pViewport->height);
		m_pEngine->scissor(m_pScissor->x, m_pScissor->y, m_pScissor->z, m_pScissor->w);
	}
}
//------------------------------------------------------------------------------
