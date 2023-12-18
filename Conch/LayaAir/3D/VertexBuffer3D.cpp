#include "VertexBuffer3D.h"

namespace laya
{
	VertexBuffer3D::VertexBuffer3D(WebGLEngine* engine, int nByteLength, BufferUsage bufferUsageType,bool bSelfCreateBuffer)
	{
		m_bSelfCreateBuffer = bSelfCreateBuffer;
		m_pVertextDeclaration = NULL;
		m_nByteLength = nByteLength;
		if (m_bSelfCreateBuffer)
		{
			m_buffer = engine->createBuffer(BufferTargetType::ARRAY_BUFFER, bufferUsageType);
			m_buffer->setDataLength(m_nByteLength);
		}
		else
		{
			m_buffer = NULL;
		}
	}
	VertexBuffer3D::~VertexBuffer3D()
	{
		if (m_bSelfCreateBuffer)
		{
			if (m_buffer != nullptr)
			{
				delete m_buffer;
				m_buffer = nullptr;
			}
		}
	}
	void VertexBuffer3D::setGLBuffer(IRenderBuffer* pBuffer)
	{
		m_buffer = pBuffer;
	}
	void VertexBuffer3D::bind()
	{
		m_buffer->bindBuffer();
	}
	void VertexBuffer3D::unbind()
	{
		m_buffer->unbindBuffer();
	}
	void VertexBuffer3D::orphanStorage()
	{
		bind();
		m_buffer->setDataLength(m_nByteLength);
	}
	void VertexBuffer3D::setData(const char* buffer, int bufferOffset/* = 0*/, int dataStartIndex/* = 0*/, int dataCount/*= Number.MAX_SAFE_INTEGER*/)
	{
		bind();
		bool needSubData = dataStartIndex != 0/* || dataCount != Number.MAX_SAFE_INTEGER*/;
		if (needSubData)
		{
			m_buffer->setData((const char*)(buffer + dataStartIndex), dataCount, bufferOffset);
			//if (this._canRead)
			//	this._buffer.set(subData, bufferOffset);
		}
		else
		{
			m_buffer->setData((const char*)(buffer), dataCount, bufferOffset);
			//if (this._canRead)
			//	this._buffer.set(new Uint8Array(buffer), bufferOffset);
		}
	}
}
//------------------------------------------------------------------------------
