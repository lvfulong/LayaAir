#include "JSSubmitScene3D.h"
#include <binder/JSBind.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>


namespace laya
{
	JSSubmitScene3D::JSSubmitScene3D(): ISubmit(SubmitType::ThreeDimension)
	{
		assert(true);
		jsbind::AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchSubmitScene3D", 128, this);
	}
	//------------------------------------------------------------------------------
	JSSubmitScene3D::JSSubmitScene3D(jsvm::Value pCallback) : ISubmit(SubmitType::ThreeDimension)
	{
		m_pJSFunctionRenderSubmit = jsbind::Persistent(pCallback);
		jsbind::AdjustAmountOfExternalAllocatedMemory(128);
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
		if (!m_pJSFunctionRenderSubmit.isValid())
		{
			m_pJSFunctionRenderSubmit.call<void>(jsbind::toLocal(this));
		}
		return 1;
	}
	void JSSubmitScene3D::recycle()
	{

	}
	//------------------------------------------------------------------------------
	void JSSubmitScene3D::exportJS(jsbind::Object& context)
	{
		jsbind::class_<JSSubmitScene3D> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<jsvm::Value>();
		context.class_("conchSubmitScene3D", class_binding);
	}
}
//------------------------------------------------------------------------------
