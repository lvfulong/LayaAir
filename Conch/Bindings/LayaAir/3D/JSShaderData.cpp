#include "JSShaderData.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>


namespace laya
{
	JSShaderData::JSShaderData()
	{
		m_pShaderData = new ShaderData(this);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchShaderData", 128, this);
	}
	//------------------------------------------------------------------------------
	JSShaderData::~JSShaderData()
	{
        m_pJSApplyUBOData.reset();
		if (m_pShaderData)
		{
			delete m_pShaderData;
			m_pShaderData = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchShaderData", this);
	}
    void JSShaderData::setApplyUBOData(JSValueAsParam obj)
    {
        m_pJSApplyUBOData.reset(obj);
    }
    void JSShaderData::applyUBOData()
    {
        m_pJSApplyUBOData.call<void>(toLocal(this));
    }

	//------------------------------------------------------------------------------
	void JSShaderData::exportJS(Context& context)
	{
		class_<JSShaderData> class_binding;
		class_binding.constructor<>();
		class_binding.property("nativeID", &JSShaderData::getNativeObjID);
        class_binding.function("destroy", &JSShaderData::destroy);
        class_binding.function("setApplyUBOData", &JSShaderData::setApplyUBOData);
		context.class_("conchShaderData", class_binding);
	}
}