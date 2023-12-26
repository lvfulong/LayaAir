#include "Buffer.h"
#include <render/3D/design/renderInterface/IRenderBuffer.h>
#include <render/driver/gles/WebGLEngine.h>

namespace laya
{
	Buffer::Buffer(WebGLEngine* engine, BufferTargetType targetType, BufferUsage bufferUsageType)
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
