#include "Buffer.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>

namespace laya
{
	Buffer::Buffer(GLESEngine* engine, BufferTargetType targetType, BufferUsage bufferUsageType)
	{
		m_glBuffer = engine->createBuffer(targetType, bufferUsageType);
		m_bufferType = targetType;
		m_bufferUsage = bufferUsageType;
	}
	Buffer::~Buffer()
	{
		if (m_glBuffer)
		{
			delete m_glBuffer;
			m_glBuffer = nullptr;
		}
	}

	bool Buffer::bind()
	{
		return m_glBuffer->bindBuffer();
	}

	void Buffer::unbind()
	{
		return m_glBuffer->unbindBuffer();
	}

	void Buffer::resizelength(int bytelength)
	{
		m_byteLength = bytelength;
		m_glBuffer->resizeBuffer(m_byteLength);
	}
}
