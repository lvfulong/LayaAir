#include "JSCommandUniformMap.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>

namespace laya
{
	JSCommandUniformMap::JSCommandUniformMap()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchCommandUniformMap", 128, this);
	}
	//------------------------------------------------------------------------------
	JSCommandUniformMap::JSCommandUniformMap(CommandUniformMap* pCommandUniformMap)
	{
		m_pCommandUniformMap = pCommandUniformMap;
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchCommandUniformMap", 128, this);
	}
	//------------------------------------------------------------------------------
	JSCommandUniformMap::~JSCommandUniformMap()
	{
		/*if (m_pCommandUniformMap)
		{
			delete m_pCommandUniformMap;
			m_pCommandUniformMap = nullptr;
		}*/
		JCMemorySurvey::GetInstance()->releaseClass("conchCommandUniformMap", this);
	}
	void JSCommandUniformMap::addShaderUniform(int id, const char* sName)
	{
		m_pCommandUniformMap->addShaderUniform(id, sName);
	}
	JsValue JSCommandUniformMap::createGlobalUniformMap(const char* name)
	{
		JSCommandUniformMap* pMap = new JSCommandUniformMap(CommandUniformMap::createGlobalUniformMap(name));
		return JSP_TO_JS(JSCommandUniformMap*, pMap);
	}
	bool JSCommandUniformMap::hasPtrID(int propertyID)
	{
		return m_pCommandUniformMap->hasPtrID(propertyID);
	}
	//------------------------------------------------------------------------------
	void JSCommandUniformMap::exportJS(Context& context)
	{
		class_<JSCommandUniformMap> class_binding;
		class_binding.constructor<>();
		class_binding.function("hasPtrID", &JSCommandUniformMap::hasPtrID);
		class_binding.function("addShaderUniform", &JSCommandUniformMap::addShaderUniform);
		class_binding.class_function("createGlobalUniformMap", &JSCommandUniformMap::createGlobalUniformMap);
		context.class_("conchCommandUniformMap", class_binding);
	}
}
//------------------------------------------------------------------------------
