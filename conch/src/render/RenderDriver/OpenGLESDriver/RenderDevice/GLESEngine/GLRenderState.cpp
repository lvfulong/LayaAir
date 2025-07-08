#include "GLRenderState.h"
#include <utils/Log.h>

namespace laya
{
	void GLRenderState::setStencilMask(bool value)
	{
		m_stencilWrite = value;
		if (value) {
			setStencilWriteMask(m_stencilWriteMask);
		}
	}

	void GLRenderState::setStencilWriteMask(GLenum value) 
	{
		m_stencilWriteMask = value;
		value = m_stencilWrite ? value : 0x00;
		if (value != m_curStencilWriteMask) {
			m_curStencilWriteMask = value;
			glStencilMask(value);
		}
	}

	void GLRenderState::setDephthBiasFactor(GLfloat constantFactor, GLfloat slopeFactor, float clamp) {
		if (constantFactor != m_depthBiasConstant || slopeFactor != m_DepthBiasSlope || clamp != m_depthBiasClamp) {
			m_depthBiasConstant = constantFactor;
			m_DepthBiasSlope = slopeFactor;
			m_depthBiasClamp = clamp;
			glPolygonOffset(constantFactor, slopeFactor);
		}
	}
}
//------------------------------------------------------------------------------
