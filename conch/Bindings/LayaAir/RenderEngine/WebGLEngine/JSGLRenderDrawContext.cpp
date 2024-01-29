#include "JSGLRenderDrawContext.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "JSGLESEngine.h"
#include "../../3D/JSRenderGeometryElement.h"
#include <render/driver/gles/GLESEngine.h>
#include <render/3D/design/renderInterface/IRenderDrawContext.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <render/3D/design/renderEnum/IndexFormat.h>

namespace laya
{

	JSGLRenderDrawContext::JSGLRenderDrawContext()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLRenderDrawContext", 128, this);
	}
	JSGLRenderDrawContext::JSGLRenderDrawContext(JSValueAsParam engine)
	{
		JSGLESEngine* pWebglEngine = (JSGLESEngine*)Converter<JSGLESEngine*>::ToCpp(engine);
		m_pEngine = pWebglEngine->m_pEngine;
		m_pRenderDrawContext = m_pEngine->getDrawContext();
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLRenderDrawContext", 128, this);
	}
	//------------------------------------------------------------------------------
	JSGLRenderDrawContext::~JSGLRenderDrawContext()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchGLRenderDrawContext", this);
	}
	void JSGLRenderDrawContext::drawElementsInstanced(int mode, int count, int type, int offset, int instanceCount)
	{
		m_pRenderDrawContext->drawElementsInstanced((MeshTopology)mode, count, (IndexFormat)type, offset, instanceCount);
	}
	void JSGLRenderDrawContext::drawArraysInstanced(int mode, int first, int count, int instanceCount)
	{
		m_pRenderDrawContext->drawArraysInstanced((MeshTopology)mode, first, count, instanceCount);
	}
	void JSGLRenderDrawContext::drawArrays(int mode, int first, int count)
	{
		m_pRenderDrawContext->drawArrays((MeshTopology)mode, first, count);
	}
	void JSGLRenderDrawContext::drawElements(int mode, int count, int type, int offset)
	{
		m_pRenderDrawContext->drawElements((MeshTopology)mode, count, (IndexFormat)type, offset);
	}
	void JSGLRenderDrawContext::drawGeometryElement(JSValueAsParam pGeometryElement)
	{
		JSRenderGeometryElement* pJSRenderGeometryElement = (JSRenderGeometryElement*)Converter<JSRenderGeometryElement*>::ToCpp(pGeometryElement);
		m_pRenderDrawContext->drawGeometryElement(pJSRenderGeometryElement->m_pRenderGeometryElement);
	}
	//------------------------------------------------------------------------------
	void JSGLRenderDrawContext::exportJS(Context& context)
	{
		class_<JSGLRenderDrawContext> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		class_binding.function("drawElementsInstanced", &JSGLRenderDrawContext::drawElementsInstanced);
		class_binding.function("drawArraysInstanced", &JSGLRenderDrawContext::drawArraysInstanced);
		class_binding.function("drawArrays", &JSGLRenderDrawContext::drawArrays);
		class_binding.function("drawElements", &JSGLRenderDrawContext::drawElements);
		class_binding.function("drawGeometryElement", &JSGLRenderDrawContext::drawGeometryElement);
		context.class_("conchGLRenderDrawContext", class_binding);
	}
}
//------------------------------------------------------------------------------
