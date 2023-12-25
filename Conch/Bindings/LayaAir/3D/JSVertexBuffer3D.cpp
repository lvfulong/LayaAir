#include "JSVertexBuffer3D.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "../RenderEngine/WebGLEngine/JSGLBuffer.h"

namespace laya
{

	JSVertexBuffer3D::JSVertexBuffer3D()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchVertexBuffer3D", 128, this);
	}
	JSVertexBuffer3D::JSVertexBuffer3D(JSValueAsParam engine, int nByteLength, int bufferUsageType,bool bSelfCreateBuffer)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(engine);
		m_pVertexBuffer3D = new VertexBuffer3D(pWebglEngine->m_pEngine, nByteLength, (BufferUsage)bufferUsageType,bSelfCreateBuffer);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchVertexBuffer3D", 128, this);
	}
	//------------------------------------------------------------------------------
	JSVertexBuffer3D::~JSVertexBuffer3D()
	{
		if (m_pVertexBuffer3D)
		{
			delete m_pVertexBuffer3D;
			m_pVertexBuffer3D = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchVertexBuffer3D", this);
	}
	void JSVertexBuffer3D::setVertexDeclaration(JSValueAsParam buffer)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pVertexBuffer3D->setVertexDeclaration((int*)data, dataLength);
		}
	}
	/*bool JSVertexBuffer3D::bindBuffer()
	{
		return m_pVertexBuffer3D->m_buffer->bindBuffer();
	}
	void JSVertexBuffer3D::unbindBuffer()
	{
		m_pVertexBuffer3D->m_buffer->unbindBuffer();
	}
	void JSVertexBuffer3D::setDataLength(int length)
	{
		m_pVertexBuffer3D->m_buffer->setDataLength(length);
	}
	void JSVertexBuffer3D::setData(JSValueAsParam buffer, int offset)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pVertexBuffer3D->m_buffer->setData(data, dataLength, offset);
		}
	}
	void JSVertexBuffer3D::setDataEx(JSValueAsParam buffer, int offset, int length)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pVertexBuffer3D->m_buffer->setDataEx(data, offset, length);
		}
	}*/
	void JSVertexBuffer3D::setGLBuffer(JSValueAsParam pBuffer)
	{
		JSGLBuffer* pJSGLBuffer = (JSGLBuffer*)Converter<JSGLBuffer*>::ToCpp(pBuffer);
		if (pJSGLBuffer && pJSGLBuffer->m_pGLBuffer)
		{
			m_pVertexBuffer3D->setGLBuffer(pJSGLBuffer->m_pGLBuffer);
		}
	}
	bool JSVertexBuffer3D::getInstanceBuffer()
	{
		return m_pVertexBuffer3D->m_bInstanceBuffer;
	}
	void JSVertexBuffer3D::setInstanceBuffer(bool value)
	{
		m_pVertexBuffer3D->m_bInstanceBuffer = value;
	}
	//------------------------------------------------------------------------------
	void JSVertexBuffer3D::exportJS(Context& context)
	{
        class_<JSVertexBuffer3D> class_binding;
		class_binding.constructor<>();
        class_binding.constructor<JSValueAsParam, int, int, bool>();
        class_binding.property("_instanceBuffer", &JSVertexBuffer3D::getInstanceBuffer, &JSVertexBuffer3D::setInstanceBuffer);
        class_binding.function("setVertexDeclaration", &JSVertexBuffer3D::setVertexDeclaration);
		//JSP_ADD_METHOD("bindBuffer", JSVertexBuffer3D::unbindBuffer);
		//JSP_ADD_METHOD("unbindBuffer", JSVertexBuffer3D::unbindBuffer);
		//JSP_ADD_METHOD("setDataLength", JSVertexBuffer3D::setDataLength);
		//JSP_ADD_METHOD("setData", JSVertexBuffer3D::setData);
		//JSP_ADD_METHOD("setDataEx", JSVertexBuffer3D::setDataEx);
        class_binding.function("setGLBuffer", &JSVertexBuffer3D::setGLBuffer);
        context.class_("conchVertexBuffer3D", class_binding);
	}
}
//------------------------------------------------------------------------------
