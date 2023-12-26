#include "IndexBuffer3D.h"
#include <cassert>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/BufferTargetType.h>

namespace laya
{
	IndexBuffer3D::IndexBuffer3D(WebGLEngine* engine, IndexFormat indexFormat, int indexCount, BufferUsage bufferUsageType, bool bSelfCreateBuffer)
	{
		m_bSelfCreateBuffer = bSelfCreateBuffer;
		m_nIndexTypeByteCount = 2;
		m_nIndexFormat = indexFormat;
		m_nIndexCount = indexCount;
		switch (indexFormat)
		{
		case IndexFormat::UInt8:
			m_nIndexTypeByteCount = 1;
			break;
		case IndexFormat::UInt16:
			m_nIndexTypeByteCount = 2;
			break;
		case IndexFormat::UInt32:
			m_nIndexTypeByteCount = 4;
			break;
		default:
			assert(true);
			break;
		}
		m_nByteLength = m_nIndexTypeByteCount*m_nIndexCount;

		if (m_bSelfCreateBuffer)
		{
			m_buffer = engine->createBuffer(BufferTargetType::ELEMENT_ARRAY_BUFFER, bufferUsageType);
			m_buffer->setDataLength(m_nByteLength);
		}
		else
		{
			m_buffer = NULL;
		}
	}
	IndexBuffer3D::~IndexBuffer3D()
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
	void IndexBuffer3D::setGLBuffer(IRenderBuffer* pBuffer)
	{
		m_buffer = pBuffer;
	}
	void IndexBuffer3D::bind()
	{
		m_buffer->bindBuffer();
	}
	void IndexBuffer3D::unbind()
	{
		m_buffer->unbindBuffer();
	}
}
//------------------------------------------------------------------------------
