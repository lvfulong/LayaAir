#include "GLRenderState.h"
#include <utils/Log.h>

namespace laya
{
	void GLRenderState::setStencilMask(bool value)
	{
		if (g_kSystemConfig.m_bConchWebGL)
		{
			value ? glStencilMask(0xff) : glStencilMask(0x00);
		}
		else
		{
			if (value != m_stencilMask)
			{
				m_stencilMask = value;
				value ? glStencilMask(0xff) : glStencilMask(0x00);
			}
		}
	}
}
//------------------------------------------------------------------------------
