#include "JSIndexBuffer3D.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "../RenderEngine/WebGLEngine/JSGLBuffer.h"

namespace laya
{

	JSIndexBuffer3D::JSIndexBuffer3D()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchIndexBuffer3D", 128, this);
	}
	JSIndexBuffer3D::JSIndexBuffer3D(JSValueAsParam engine, int nIndexFormat, int nIndexCount, int bufferUsageType, bool bSelfCreateBuffer)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(engine);
		m_pIndexBuffer3D = new IndexBuffer3D(pWebglEngine->m_pEngine, (IndexFormat)nIndexFormat, nIndexCount,(BufferUsage)bufferUsageType, bSelfCreateBuffer);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchIndexBuffer3D", 128, this);
	}
	//------------------------------------------------------------------------------
	JSIndexBuffer3D::~JSIndexBuffer3D()
	{
		if (m_pIndexBuffer3D)
		{
			delete m_pIndexBuffer3D;
			m_pIndexBuffer3D = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchIndexBuffer3D", this);
	}
	/*bool JSIndexBuffer3D::bindBuffer()
	{
		return m_pIndexBuffer3D->m_buffer->bindBuffer();
	}
	void JSIndexBuffer3D::unbindBuffer()
	{
		m_pIndexBuffer3D->m_buffer->unbindBuffer();
	}
	void JSIndexBuffer3D::orphanStorage()
	{
		m_pIndexBuffer3D->m_buffer->orphanStorage();
	}
	void JSIndexBuffer3D::setDataLength(int length)
	{
		m_pIndexBuffer3D->m_buffer->setDataLength(length);
	}
	void JSIndexBuffer3D::setData(JSValueAsParam buffer, int offset)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pIndexBuffer3D->m_buffer->setData(data, dataLength, offset);
		}
	}
	void JSIndexBuffer3D::setDataEx(JSValueAsParam buffer, int offset, int length)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pIndexBuffer3D->m_buffer->setDataEx(data, offset, length);
		}
	}*/
	void JSIndexBuffer3D::setGLBuffer(JSValueAsParam pBuffer)
	{
		JSGLBuffer* pJSGLBuffer = (JSGLBuffer*)Converter<JSGLBuffer*>::ToCpp(pBuffer);
		if (pJSGLBuffer && pJSGLBuffer->m_pGLBuffer)
		{
			m_pIndexBuffer3D->setGLBuffer(pJSGLBuffer->m_pGLBuffer);
		}
	}
	//------------------------------------------------------------------------------
	void JSIndexBuffer3D::exportJS(Context& context)
	{
		class_<JSIndexBuffer3D> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam, int, int,int,bool>();
		//JSP_ADD_METHOD("bindBuffer", JSIndexBuffer3D::unbindBuffer);
		//JSP_ADD_METHOD("unbindBuffer", JSIndexBuffer3D::unbindBuffer);
		//JSP_ADD_METHOD("orphanStorage", JSIndexBuffer3D::orphanStorage);
		//JSP_ADD_METHOD("setDataLength", JSIndexBuffer3D::setDataLength);
		//JSP_ADD_METHOD("setData", JSIndexBuffer3D::setData);
		//JSP_ADD_METHOD("setDataEx", JSIndexBuffer3D::setDataEx);
		class_binding.function("setGLBuffer", &JSIndexBuffer3D::setGLBuffer);
		context.class_("conchIndexBuffer3D", class_binding);
	}
}
//------------------------------------------------------------------------------
