#include "JSUniformBufferObject.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../RenderEngine/GLESEngine/JSGLESEngine.h"
#include "../RenderEngine/GLESEngine/JSGLBuffer.h"

namespace laya
{
	JSUniformBufferObject::JSUniformBufferObject()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchUniformBufferObject", 128, this);
	}
	JSUniformBufferObject::JSUniformBufferObject(JSValueAsParam engine, int glPointer)
	{
		JSGLESEngine* pWebglEngine = (JSGLESEngine*)Converter<JSGLESEngine*>::ToCpp(engine);
		m_pUniformBufferObject = new UniformBufferObject(pWebglEngine->m_pEngine, glPointer, false);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchUniformBufferObject", 128, this);
	}
	//------------------------------------------------------------------------------
	JSUniformBufferObject::~JSUniformBufferObject()
	{
		if (m_pUniformBufferObject)
		{
			delete m_pUniformBufferObject;
			m_pUniformBufferObject = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchUniformBufferObject", this);
	}
	void JSUniformBufferObject::setGLBuffer(JSValueAsParam pBuffer)
	{
		JSGLBuffer* pJSGLBuffer = (JSGLBuffer*)Converter<JSGLBuffer*>::ToCpp(pBuffer);
		if (pJSGLBuffer && pJSGLBuffer->m_pGLBuffer)
		{
			m_pUniformBufferObject->setGLBuffer(pJSGLBuffer->m_pGLBuffer);
		}
	}
	int JSUniformBufferObject::getNativeObjID()
	{
		return m_pUniformBufferObject->getID();
	}
	//------------------------------------------------------------------------------
	void JSUniformBufferObject::exportJS(Context& context)
	{
		class_<JSUniformBufferObject> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam, int>();
		class_binding.property("nativeID", &JSUniformBufferObject::getNativeObjID);
		class_binding.function("setGLBuffer", &JSUniformBufferObject::setGLBuffer);
		context.class_("conchUniformBufferObject", class_binding);
	}
}
//------------------------------------------------------------------------------
