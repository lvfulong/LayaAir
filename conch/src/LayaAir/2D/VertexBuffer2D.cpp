#include "VertexBuffer2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>

namespace laya
{
	//------------------------------------------------------------------------------
	VertexBuffer2D::VertexBuffer2D(GLESEngine* engine, int vertexStride, BufferUsage bufferUsage): Buffer(engine, BufferTargetType::ARRAY_BUFFER, bufferUsage)
	{
		m_buffer2D = new Buffer2D(this);
		m_vertexStride = vertexStride;
		//m_bufferUsage = bufferUsage;
	}
	VertexBuffer2D::~VertexBuffer2D()
	{
		if (m_buffer2D)
		{
			delete m_buffer2D;
			m_buffer2D = nullptr;
		}
	}
	void VertexBuffer2D::_bindForVAO()
	{
		m_glBuffer->bindBuffer();
	}

	//------------------------------------------------------------------------------
}
