#include "JSCommandEncoder.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>

namespace laya
{

	JSCommandEncoder::JSCommandEncoder()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(102400);
		JCMemorySurvey::GetInstance()->newClass("conchCommandEncoder", 102400, this);
	}
	//------------------------------------------------------------------------------
	JSCommandEncoder::JSCommandEncoder(CommandEncoder* pCommandEncoder)
	{
		m_pCommandEncoder = pCommandEncoder;
		AdjustAmountOfExternalAllocatedMemory(102400);
		JCMemorySurvey::GetInstance()->newClass("conchCommandEncoder", 102400, this);
	}
	//------------------------------------------------------------------------------
	JSCommandEncoder::~JSCommandEncoder()
	{
		if (m_pCommandEncoder)
		{
			delete m_pCommandEncoder;
			m_pCommandEncoder = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchCommandEncoder", this);
	}
	//------------------------------------------------------------------------------
	void JSCommandEncoder::exportJS(Context& context)
	{
		class_<JSCommandEncoder> class_binding;
		class_binding.constructor<>();
		context.class_("conchCommandEncoder", class_binding);
	}
}
//------------------------------------------------------------------------------
