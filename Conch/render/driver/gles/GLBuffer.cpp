#include "GLBuffer.h"
#include "GLEnum/WebGLExtension.h"
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include <Bindings/JSConchConfig.h>
#include "WebGLEngine.h"

namespace laya
{
	GLBuffer::GLBuffer(WebGLEngine* engine, BufferTargetType targetType , BufferUsage bufferUsageType): GLObject(engine)
	{
		m_targetType = targetType;
		m_bufferUsageType = bufferUsageType;
		_getGLTarget(m_targetType);
		_getGLUsage(m_bufferUsageType);
		glGenBuffers(1, &m_glBuffer);
	}
	GLBuffer::~GLBuffer()
	{
		destroy();
	}
	void GLBuffer::_memorychange(int bytelength)
	{
		m_engine->addStatisticsInfo(RenderStatisticsInfo::BufferMemory, bytelength);
		m_engine->addStatisticsInfo(RenderStatisticsInfo::GPUMemory, bytelength);
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
		_memorychange(-m_byteLength);
		m_byteLength = length;
		glBufferData(m_glTarget,m_byteLength,NULL,m_glUsage);
		unbindBuffer();
		_memorychange(m_byteLength);
	}
	void GLBuffer::setData(const char* buffer, int bufferLength, int offset)
	{
		bindBuffer();
		glBufferSubData(m_glTarget, offset, bufferLength, buffer);
		unbindBuffer();
	}
	void GLBuffer::setDataEx(const char* buffer, int offset, int length)
	{
		bindBuffer();
		glBufferSubData(m_glTarget, offset, length, buffer);
		unbindBuffer();
	}
	void GLBuffer::bindBufferBase(int glPointer)
	{
		//if (m_engine->_getBindUBOBuffer(glPointer) != this)
		//{
			glBindBufferBase(m_glTarget, glPointer, m_glBuffer);
			//m_engine->_setBindUBOBuffer(glPointer, this);
		//}

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
			glDeleteBuffers(1, &m_glBuffer);
			_memorychange(m_byteLength);
			m_byteLength = 0;
			m_engine = nullptr;
			m_glBuffer = 0;
			m_glTarget = 0;
			m_glUsage = 0;
		}
	}
}