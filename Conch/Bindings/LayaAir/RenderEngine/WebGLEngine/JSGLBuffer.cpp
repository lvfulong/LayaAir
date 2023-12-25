#include "JSGLBuffer.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "JSWebGLEngine.h"

namespace laya
{
	JSGLBuffer::JSGLBuffer()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLBuffer", 128, this);
	}
	JSGLBuffer::JSGLBuffer(JSValueAsParam engine, int bufferTargetType, int bufferUsageType)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(engine);
		m_pGLBuffer = new GLBuffer(pWebglEngine->m_pEngine, BufferTargetType(bufferTargetType), BufferUsage(bufferUsageType));
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLBuffer", 128, this);
	}
	//------------------------------------------------------------------------------
	JSGLBuffer::~JSGLBuffer()
	{
		if (m_pGLBuffer)
		{
			delete m_pGLBuffer;
			m_pGLBuffer = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchGLBuffer", this);
	}
	bool JSGLBuffer::bindBuffer()
	{
		return m_pGLBuffer->bindBuffer();
	}
	void JSGLBuffer::unbindBuffer()
	{
		m_pGLBuffer->unbindBuffer();
	}
	void JSGLBuffer::orphanStorage()
	{
		m_pGLBuffer->orphanStorage();
	}
	void JSGLBuffer::setDataLength(int length)
	{
		m_pGLBuffer->setDataLength(length);
	}
	void JSGLBuffer::setData(JSValueAsParam buffer, int bufferOffset)
	{
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
			m_pGLBuffer->setData(data, dataLength, bufferOffset);
		}
	}
	void JSGLBuffer::setDataEx(JSValueAsParam buffer, int offset, int length)
	{
		//TODO test
		char* data = NULL;
		int dataLength = 0;
		bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
		if (bIsArrayBuffer)
		{
                if (buffer->IsUint8Array() ||
                    buffer->IsUint8ClampedArray() ||
                    buffer->IsInt8Array())
                {
                    m_pGLBuffer->setDataEx(data, offset, length * 1);
                }
                else if (buffer->IsUint16Array() ||
                         buffer->IsInt16Array())
                {
                    m_pGLBuffer->setDataEx(data, offset, length * 2);
                }
                else if (buffer->IsUint32Array() ||
                         buffer->IsInt32Array() ||
                         buffer->IsFloat32Array())
                {
                    m_pGLBuffer->setDataEx(data, offset, length * 4);
                }
                else if (buffer->IsFloat64Array())
                {
                    m_pGLBuffer->setDataEx(data, offset, length * 8);
                }
                else
                {
                    LOGE("JSGLBuffer::setDataEx type error");
                }
		}
	}
	void JSGLBuffer::bindBufferBase(int glPointer)
	{
		m_pGLBuffer->bindBufferBase(glPointer);
	}
	void JSGLBuffer::bindBufferRange(int glPointer, int offset, int byteCount)
	{
		m_pGLBuffer->bindBufferRange(glPointer, offset, byteCount);
	}
	void JSGLBuffer::resizeBuffer(int length)
	{
		m_pGLBuffer->resizeBuffer(length);
	}
	void JSGLBuffer::destroy()
	{
		m_pGLBuffer->destroy();
	}
	//------------------------------------------------------------------------------
	void JSGLBuffer::exportJS(Context& context)
	{
		class_<JSGLBuffer> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam, int, int>();
		class_binding.function("bindBuffer", &JSGLBuffer::bindBuffer);
		class_binding.function("unbindBuffer", &JSGLBuffer::unbindBuffer);
		class_binding.function("orphanStorage", &JSGLBuffer::orphanStorage);
		class_binding.function("setDataLength", &JSGLBuffer::setDataLength);
		class_binding.function("setData", &JSGLBuffer::setData);
		class_binding.function("setDataEx", &JSGLBuffer::setDataEx);
		class_binding.function("bindBufferBase", &JSGLBuffer::bindBufferBase);
		class_binding.function("bindBufferRange", &JSGLBuffer::bindBufferRange);
		class_binding.function("resizeBuffer", &JSGLBuffer::resizeBuffer);
		class_binding.function("destroy", &JSGLBuffer::destroy);
		context.class_("conchGLBuffer", class_binding);
	}
}
//------------------------------------------------------------------------------
