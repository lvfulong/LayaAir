#include "JSAttributeMap.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>

namespace laya
{
	JSAttributeMap::JSAttributeMap()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchAttributeMap", 128, this);
	}
	//------------------------------------------------------------------------------
	JSAttributeMap::~JSAttributeMap()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchAttributeMap", this);
	}
	void JSAttributeMap::setAttributeValue(const char* sName, int nLocation)
	{
		m_pAttributeMap.setValue(sName, nLocation);
	}
	void JSAttributeMap::setStateValue(int key, int value)
	{
		m_pStateMap.setValue(key, value);
	}
	void JSAttributeMap::setShaderPassStatefirst(bool value)
	{
		m_pStateMap.m_shaderPassStatefirst = value;
	}
	bool JSAttributeMap::getShaderPassStatefirst()
	{
		return m_pStateMap.m_shaderPassStatefirst;
	}
	//------------------------------------------------------------------------------
	void JSAttributeMap::exportJS(Context& context)
	{
		class_<JSAttributeMap> class_binding;
		class_binding.constructor<>();
		class_binding.property("statefirst", &JSAttributeMap::getShaderPassStatefirst, &JSAttributeMap::setShaderPassStatefirst);
		class_binding.function("setAttributeValue", &JSAttributeMap::setAttributeValue);
		class_binding.function("setStateValue", &JSAttributeMap::setStateValue);
		context.class_("conchAttributeMap", class_binding);
	}
}
//------------------------------------------------------------------------------
