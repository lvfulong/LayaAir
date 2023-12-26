#include "GLRenderState.h"
#include <utils/Log.h>

namespace laya
{
	void GLRenderState::applyRenderStateCommand(const RenderStateCommand& cmd) 
	{
		for (int i = 0, n = cmd.m_vRenderStateCommandItems.size(); i < n; i++)
		{
			const RenderStateCommandItem& command = cmd.m_vRenderStateCommandItems[i];
			switch (command.m_nRenderState)
			{
			case RenderStateType::DepthTest:
				setDepthTest(command.m_nValue0);
				break;
			case RenderStateType::DepthMask:
				setDepthMask(command.m_nValue0);
				break;
			case RenderStateType::DepthFunc:
				setDepthFunc((CompareFunction)(command.m_nValue0));
				break;
			case RenderStateType::StencilTest:
				setStencilTest(command.m_nValue0);
				break;
			case RenderStateType::StencilMask:
				setStencilMask(command.m_nValue0);
				break;
			case RenderStateType::StencilFunc:
				setStencilFunc((CompareFunction)command.m_nValue0, command.m_nValue1);
				break;
			case RenderStateType::StencilOp:
				setstencilOp((StencilOperation)command.m_nValue0, (StencilOperation)command.m_nValue1, (StencilOperation)command.m_nValue2);//TODO
				break;
			case RenderStateType::BlendType:
				setBlend(command.m_nValue0 != (int)BlendType::BLEND_DISABLE);
				break;
			case RenderStateType::BlendEquation:
				setBlendEquation((BlendEquationSeparate)command.m_nValue0);
				break;
			case RenderStateType::BlendEquationSeparate:
				setBlendEquationSeparate((BlendEquationSeparate)command.m_nValue0, (BlendEquationSeparate)command.m_nValue1);//TODO
				break;
			case RenderStateType::BlendFunc:
				setBlendFunc((BlendFactor)command.m_nValue0, (BlendFactor)command.m_nValue1);
				break;
			case RenderStateType::BlendFuncSeperate:
				setBlendFuncSeperate((BlendFactor)command.m_nValue0, (BlendFactor)command.m_nValue1, (BlendFactor)command.m_nValue2, (BlendFactor)command.m_nValue3);
				break;
			case RenderStateType::CullFace:
				setCullFace(command.m_nValue0);
				break;
			case RenderStateType::FrontFace:
				setFrontFace((CullMode)command.m_nValue0);
				break;
			default:
				LOGI("---------------unknow type of renderStateType %d", (int)RenderStateType::FrontFace);
				assert("unknow type of renderStateType");
				break;
			}
		}
	}
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
