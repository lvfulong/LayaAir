#include "GLBuffer.h"
#include "GLEnum/WebGLExtension.h"
#include <utils/Preprocessor.h>
#include <Bindings/JSConchConfig.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "render/LayaGL.h"

namespace laya
{
	GLBuffer::GLBuffer(GLESEngine* engine, BufferTargetType targetType , BufferUsage bufferUsageType): GLObject(engine)
	{
		m_targetType = targetType;
		m_bufferUsageType = bufferUsageType;
		_getGLTarget(m_targetType);
		_getGLUsage(m_bufferUsageType);
		glGenBuffers(1, &m_glBuffer);
		m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_GPUBuffer, 1);
	}
	GLBuffer::~GLBuffer()
	{
		destroy();
	}

	void GLBuffer::_getGLUsage(BufferUsage usage)
	{
		switch (usage) 
		{
		case BufferUsage::Static:
			m_glUsage = GL_STATIC_DRAW;
			break;
		case BufferUsage::Dynamic:
			m_glUsage = GL_DYNAMIC_DRAW;
			break;
		case BufferUsage::Stream:
			m_glUsage = GL_STREAM_DRAW;
			break;
		default:
			LOGW("usage is not standard");
			break;
		}
	}

	void GLBuffer::_getGLTarget(BufferTargetType target)
	{
		switch (target)
		{
		case BufferTargetType::ARRAY_BUFFER:
			m_glTarget = GL_ARRAY_BUFFER;
			break;
		case BufferTargetType::UNIFORM_BUFFER:
			m_glTarget = GL_UNIFORM_BUFFER;
			break;
		case BufferTargetType::ELEMENT_ARRAY_BUFFER:
			m_glTarget = GL_ELEMENT_ARRAY_BUFFER;
			break;
		default:
			break;
		}
	}
	void GLBuffer::_memorychange(int bytelength)
	{
        if (LayaGL::m_pWebglEngine != nullptr)
        {
            LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::M_GPUBuffer, -m_byteLength+ bytelength);
        }
	}
	bool GLBuffer::bindBuffer()
	{
		if (m_engine->_getbindBuffer(m_targetType) != this) 
		{
			glBindBuffer(m_glTarget, m_glBuffer);
			m_engine->_setbindBuffer(m_targetType, this);
			return true;
		}
		return false;
	}

	void GLBuffer::unbindBuffer()
	{
		if (m_engine->_getbindBuffer(m_targetType) == this)
		{
			glBindBuffer(m_glTarget, 0);
			m_engine->_setbindBuffer(m_targetType, 0);
		}
	}

	void GLBuffer::orphanStorage()
	{
		bindBuffer();
		setDataLength(m_byteLength);
	}
	void GLBuffer::setDataLength(int length)
	{
		bindBuffer();
		_memorychange(length);
		m_byteLength = length;
		glBufferData(m_glTarget,m_byteLength,NULL,m_glUsage);
		unbindBuffer();
	}
	void GLBuffer::setData(const char* buffer, int bufferLength, int offset)
	{
		bindBuffer();
		glBufferSubData(m_glTarget, offset, bufferLength, buffer);
		m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_GeometryBufferUploadCount, 1);
		unbindBuffer();
	}
	void GLBuffer::setDataEx(const char* buffer, int offset, int length)
	{
		bindBuffer();
		glBufferSubData(m_glTarget, offset, length, buffer);
		m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_GeometryBufferUploadCount, 1);
		unbindBuffer();
	}
	void GLBuffer::bindBufferBase(int glPointer)
	{
		//LayaMe 时代闪屏 注释这个检测好了
		if (m_engine->_getBindUBOBuffer(glPointer) != this)
		{
			glBindBufferBase(m_glTarget, glPointer, m_glBuffer);
			m_engine->_setBindUBOBuffer(glPointer, this);
		}

	}
	//TODO:
	void GLBuffer::bindBufferRange(int glPointer, int offset, int byteCount)
	{
		glBindBufferRange(m_glTarget, glPointer, m_glBuffer, offset, byteCount);
	}
	void GLBuffer::resizeBuffer(int dataLength)
	{
		bindBuffer();
		m_byteLength = dataLength;
		glBufferData(m_glTarget, m_byteLength, nullptr, m_glUsage);
	}
	void GLBuffer::destroy()
	{
		if (m_glBuffer != 0)
		{
            if (LayaGL::m_pWebglEngine != nullptr)
            {
                LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_GPUBuffer, -1);
            }
			glDeleteBuffers(1, &m_glBuffer);
			_memorychange(0);
			m_byteLength = 0;
			m_engine = nullptr;
			m_glBuffer = 0;
			m_glTarget = 0;
			m_glUsage = 0;
		}
	}
}
