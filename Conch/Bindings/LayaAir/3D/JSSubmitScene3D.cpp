#include "JSSubmitScene3D.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>


namespace laya
{
	JSSubmitScene3D::JSSubmitScene3D(): ISubmit(SubmitType::ThreeDimension)
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchSubmitScene3D", 128, this);
	}
	//------------------------------------------------------------------------------
	JSSubmitScene3D::JSSubmitScene3D(JSValueAsParam pCallback) : ISubmit(SubmitType::ThreeDimension)
	{
		m_pJSFunctionRenderSubmit.reset(pCallback);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchSubmitScene3D", 128, this);
	}
	//------------------------------------------------------------------------------
	JSSubmitScene3D::~JSSubmitScene3D()
	{
		m_pJSFunctionRenderSubmit.reset();
		JCMemorySurvey::GetInstance()->releaseClass("conchSubmitScene3D", this);
	}
	int JSSubmitScene3D::renderSubmit()
	{
		if (!m_pJSFunctionRenderSubmit.isEmpty())
		{
			m_pJSFunctionRenderSubmit.call<void>(toLocal(this));
		}
		return 1;
	}
	void JSSubmitScene3D::recycle()
	{

	}
	//------------------------------------------------------------------------------
	void JSSubmitScene3D::exportJS(Context& context)
	{
		class_<JSSubmitScene3D> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		context.class_("conchSubmitScene3D", class_binding);
	}
}
//------------------------------------------------------------------------------
