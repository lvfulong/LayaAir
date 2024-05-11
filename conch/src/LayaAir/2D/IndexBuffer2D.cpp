#include "IndexBuffer2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>

namespace laya
{
	//------------------------------------------------------------------------------
	IndexBuffer2D::IndexBuffer2D(GLESEngine* engine, BufferUsage bufferUsage):Buffer(engine, BufferTargetType::ELEMENT_ARRAY_BUFFER, bufferUsage)
	{
		m_buffer2D = new Buffer2D(this);
		//this._bufferUsage = bufferUsage;
		//this._buffer = new Uint8Array(8);
	}
	IndexBuffer2D::~IndexBuffer2D()
	{
		if (m_buffer2D)
		{
			delete m_buffer2D;
			m_buffer2D = nullptr;
		}
	}
	void IndexBuffer2D::_bindForVAO()
	{
		m_glBuffer->bindBuffer();
	}
	//------------------------------------------------------------------------------
}

