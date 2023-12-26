#include "Buffer2D.h"
#include <render/3D/design/renderInterface/IRenderBuffer.h>

namespace laya
{
	Buffer2D::Buffer2D(Buffer* buffer)
	{
		m_constBuffer = buffer;
	}
	Buffer2D::~Buffer2D()
	{
	}
	int Buffer2D::getBufferLength()
	{
		return m_constBuffer->m_buffer.size();
	}
	// �൱��setUploadSize()
	void Buffer2D::setByteLength(int value)
	{
		if (m_constBuffer->m_byteLength != value) 
		{
			if (value > m_bufferSize)
			{
				_resizeBuffer(value * 2 + 256, true);
			}
			m_constBuffer->m_byteLength = value;
		}
	}
	int Buffer2D::needSize(int sz)
	{
		int old = m_constBuffer->m_byteLength;
		if (sz)
		{
			int needsz = old + sz;
			if (needsz > m_bufferSize) 
			{
				_resizeBuffer(needsz << 1, true);
			}
			m_constBuffer->m_byteLength = needsz;
		}
		return old;
	}
	Buffer2D& Buffer2D::_resizeBuffer(int nsz, bool copy)//�Ƿ��޸��˳���
	{
		if (nsz <= m_constBuffer->m_buffer.size())
			return *this;
		if (copy && !m_constBuffer->m_buffer.empty())
		{
			m_constBuffer->m_buffer.resize(nsz);
		}
		else
		{
			m_constBuffer->m_buffer.resize(0);
			m_constBuffer->m_buffer.resize(nsz);
		}
		m_upload = true;
		m_bufferSize = m_constBuffer->m_buffer.size();
		return *this;
	}
	void Buffer2D::_bufferData()
	{
		if (m_uploadSize < m_constBuffer->m_buffer.size())
		{
			m_uploadSize = m_constBuffer->m_buffer.size();
			m_constBuffer->m_glBuffer->setDataLength(m_uploadSize);
		}
		m_constBuffer->m_glBuffer->setData((const char*)&m_constBuffer->m_buffer[0], m_constBuffer->m_byteLength, 0);
		m_constBuffer->unbind();
	}
	bool Buffer2D::_bind_upload()
	{
		if (!m_upload)
			return false;
		m_upload = false;
		m_constBuffer->bind();
		_bufferData();
		return true;
	}
}
//------------------------------------------------------------------------------
