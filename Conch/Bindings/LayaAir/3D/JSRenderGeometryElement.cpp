#include "JSRenderGeometryElement.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../RenderEngine/WebGLEngine/JSGLVertexState.h"

namespace laya
{
	JSRenderGeometryElement::JSRenderGeometryElement()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderGeometryElement", 128, this);
	}
	JSRenderGeometryElement::JSRenderGeometryElement(int nMeshPologyType, int nDrawType)
	{
		m_pRenderGeometryElement = new RenderGeometryElement((MeshTopology)nMeshPologyType, (DrawType)nDrawType);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderGeometryElement", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderGeometryElement::~JSRenderGeometryElement()
	{
		destroy();
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderGeometryElement", this);
	}
	void JSRenderGeometryElement::setDrawArrayParams(int first, int count)
	{
		m_pRenderGeometryElement->setDrawArrayParams(first, count);
	}
	void JSRenderGeometryElement::setDrawElemenParams(int count, int offset)
	{
		m_pRenderGeometryElement->setDrawElemenParams(count, offset);
	}
	void JSRenderGeometryElement::clearRenderParams()
	{
		m_pRenderGeometryElement->clearRenderParams();
	}
	void JSRenderGeometryElement::destroy()
	{
		m_pJSBufferState3D.reset();
		if (m_pRenderGeometryElement)
		{
			delete m_pRenderGeometryElement;
			m_pRenderGeometryElement = nullptr;
		}
	}
	void JSRenderGeometryElement::setMeshTopology(int nMode)
	{
		m_pRenderGeometryElement->m_nRenderMode = (MeshTopology)nMode;
	}
	int JSRenderGeometryElement::getMeshTopology()
	{
		return (int)m_pRenderGeometryElement->m_nRenderMode;
	}
	void JSRenderGeometryElement::setDrawType(int nDrawType)
	{
		m_pRenderGeometryElement->m_nDrawType = (DrawType)nDrawType;
	}
	int JSRenderGeometryElement::getDrawType()
	{
		return (int)m_pRenderGeometryElement->m_nDrawType;
	}
	void JSRenderGeometryElement::setInstanceCount(int number)
	{
		m_pRenderGeometryElement->m_nInstanceCount = number;
	}
	int JSRenderGeometryElement::getInstanceCount()
	{
		return m_pRenderGeometryElement->m_nInstanceCount;
	}
	void JSRenderGeometryElement::setIndexFormat(int indexFormat)
	{
		m_pRenderGeometryElement->m_nIndexFormat = (IndexFormat)indexFormat;
	}
	int JSRenderGeometryElement::getIndexFormat()
	{
		return (int)m_pRenderGeometryElement->m_nIndexFormat;
	}
	void JSRenderGeometryElement::setBufferState3D(JSValueAsParam bufferState3D)
	{
		JSGLVertexState* pVAO = (JSGLVertexState*)Converter<JSGLVertexState*>::ToCpp(bufferState3D);
		if (pVAO && pVAO->m_GLVertexState)
		{
			m_pJSBufferState3D.reset(bufferState3D);
			m_pRenderGeometryElement->m_pBufferState3D = pVAO->m_GLVertexState;
		}
		else
		{
			m_pJSBufferState3D.reset();
            if (m_pRenderGeometryElement != nullptr)
            {
                m_pRenderGeometryElement->m_pBufferState3D = nullptr;
            }
		}
	}
	JsValue JSRenderGeometryElement::getBufferState3D()
	{
		return m_pJSBufferState3D.toLocal().handle_;
	}
	//------------------------------------------------------------------------------
	void JSRenderGeometryElement::exportJS(Context& context)
	{
		class_<JSRenderGeometryElement> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<int,int>();
		class_binding.property("bufferState", &JSRenderGeometryElement::getBufferState3D, &JSRenderGeometryElement::setBufferState3D);
		class_binding.property("mode", &JSRenderGeometryElement::getMeshTopology, &JSRenderGeometryElement::setMeshTopology);
		class_binding.property("drawType", &JSRenderGeometryElement::getDrawType, &JSRenderGeometryElement::setDrawType);
		class_binding.property("instanceCount", &JSRenderGeometryElement::getInstanceCount, &JSRenderGeometryElement::setInstanceCount);
		class_binding.property("indexFormat", &JSRenderGeometryElement::getIndexFormat, &JSRenderGeometryElement::setIndexFormat);
		class_binding.function("setDrawArrayParams", &JSRenderGeometryElement::setDrawArrayParams);
		class_binding.function("setDrawElemenParams", &JSRenderGeometryElement::setDrawElemenParams);
		class_binding.function("clearRenderParams", &JSRenderGeometryElement::clearRenderParams);
		class_binding.function("destroy", &JSRenderGeometryElement::destroy);
		context.class_("conchRenderGeometryElement", class_binding);
	}
}
//------------------------------------------------------------------------------
