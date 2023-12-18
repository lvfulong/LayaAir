#include "JSRenderStateCommand.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../LayaAir/3D/RenderStateCommand.h"

namespace laya
{
	JSRenderStateCommand::JSRenderStateCommand()
	{
		m_pRenderStateCommand = new RenderStateCommand();
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderStateCommand", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderStateCommand::~JSRenderStateCommand()
	{
		if (m_pRenderStateCommand)
		{
			delete m_pRenderStateCommand;
			m_pRenderStateCommand = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderStateCommand", this);
	}
	void JSRenderStateCommand::addCMDInt1(int renderstate, int value0)
	{
		m_pRenderStateCommand->addCMDInt1((RenderStateType)renderstate, value0);
	}
	void JSRenderStateCommand::addCMDInt2(int renderstate, int value0, int value1)
	{
		m_pRenderStateCommand->addCMDInt2((RenderStateType)renderstate, value0, value1);
	}
	void JSRenderStateCommand::addCMDInt3(int renderstate, int value0, int value1, int value2)
	{
		m_pRenderStateCommand->addCMDInt3((RenderStateType)renderstate, value0, value1, value2);
	}
	void JSRenderStateCommand::addCMDInt4(int renderstate, int value0, int value1, int value2, int value3)
	{
		m_pRenderStateCommand->addCMDInt4((RenderStateType)renderstate, value0, value1, value2, value3);
	}
	void JSRenderStateCommand::clear()
	{
		m_pRenderStateCommand->clear();
	}
	//------------------------------------------------------------------------------
	void JSRenderStateCommand::exportJS(Context& context)
	{
		class_<JSRenderStateCommand> class_binding;
		class_binding.constructor<>();
		class_binding.function("addCMDInt1", &JSRenderStateCommand::addCMDInt1);
		class_binding.function("addCMDInt2", &JSRenderStateCommand::addCMDInt2);
		class_binding.function("addCMDInt3", &JSRenderStateCommand::addCMDInt3);
		class_binding.function("addCMDInt4", &JSRenderStateCommand::addCMDInt4);
		class_binding.function("clear", &JSRenderStateCommand::clear);
		context.class_("conchRenderStateCommand", class_binding);
	}
}
//------------------------------------------------------------------------------
