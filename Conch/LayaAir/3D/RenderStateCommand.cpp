#include "RenderStateCommand.h"
#include "../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"

namespace laya
{
	RenderStateCommand::RenderStateCommand()
	{
		m_vRenderStateCommandItems.reserve(4);
	}
	RenderStateCommand::~RenderStateCommand()
	{
		m_vRenderStateCommandItems.clear();
	}
	void RenderStateCommand::addCMDInt1(RenderStateType renderstate, int value0)
	{
		m_vRenderStateCommandItems.emplace_back(renderstate, value0);
	}
	void RenderStateCommand::addCMDInt2(RenderStateType renderstate, int value0, int value1)
	{
		m_vRenderStateCommandItems.emplace_back(renderstate, value0, value1);
	}
	void RenderStateCommand::addCMDInt3(RenderStateType renderstate, int value0, int value1, int value2)
	{
		m_vRenderStateCommandItems.emplace_back(renderstate, value0, value1, value2);
	}
	void RenderStateCommand::addCMDInt4(RenderStateType renderstate, int value0, int value1, int value2, int value3)
	{
		m_vRenderStateCommandItems.emplace_back(renderstate, value0, value1, value2, value3);
	}
	void RenderStateCommand::clear()
	{
		m_vRenderStateCommandItems.clear();
	}
}
//------------------------------------------------------------------------------
