#include <utils/JCBuffer.h>
#include <utils/Log.h>

namespace laya
{
	JCCommandEncoderBuffer::JCCommandEncoderBuffer( unsigned int nSize, unsigned int nAdjustSize)
	{
        m_bNeedDelBuffer = true;
        m_pBuffer=NULL;
		m_nBufSize=0;
		m_nDataSize=0;
		m_nAdjustSize= nAdjustSize;
		m_nReadPos=0;
		m_bChanged=true;
        if (nSize > 0)
        {
            expand(nSize);
        }
	}
	JCCommandEncoderBuffer::~JCCommandEncoderBuffer()
	{
        if ( m_bNeedDelBuffer && m_pBuffer )
        {
            delete[] m_pBuffer;
            m_pBuffer = 0;
        }
	}
	void JCCommandEncoderBuffer::setBufferExpandStep(int nAdjustSize)
	{
        m_nAdjustSize = nAdjustSize;
	}
    void JCCommandEncoderBuffer::setAlign(bool bAlign)
    {
        m_bAlign = bAlign;
    }
    void JCCommandEncoderBuffer::setBufferSize(int nBufferSize)
    {
        if (nBufferSize < 0)
        {
            return;
        }
        if (nBufferSize >= (int)m_nBufSize)
        {
            expand(nBufferSize - m_nBufSize);
        }
    }
	void JCCommandEncoderBuffer::expand(int nExpandSize)
	{
		if (nExpandSize<1 ) return;
		unsigned long nSize= nExpandSize+m_nBufSize+m_nAdjustSize;	//��Ҫ��4����ô
		char* pBuf=new char[nSize];
		if (!pBuf)
        {
			return;
		}
		if (m_pBuffer)
        {
			memcpy(pBuf, m_pBuffer,m_nBufSize);
			delete [] m_pBuffer;
		}
		m_nBufSize=nSize;
        m_pBuffer=pBuf;
		m_bChanged=true;
	}
    //------------------------------------------------------------------------------
	JCBuffer::JCBuffer()
    {
		m_bNeedDel=false;
		m_pPtr = 0;
		m_nLen = 0;
	}
    JCBuffer::JCBuffer(int p_nSize )
    {
		m_nLen=p_nSize;
		m_pPtr = new char [p_nSize];
		m_bNeedDel = true;
	}
    JCBuffer::JCBuffer(void* p_ptr, int p_nLength, bool p_bCpy, bool p_bManage)
    {
		m_bNeedDel = p_bManage;
		if(p_bCpy)
        {
			m_bNeedDel = true;
		}
		m_pPtr = (char*)p_ptr;
		m_nLen = p_nLength;
	}
    JCBuffer::JCBuffer(char* p_pStr, encode p_end)
    {
	}
    JCBuffer::~JCBuffer()
    {
		if(m_bNeedDel)
			delete [] m_pPtr;
		m_pPtr = 0;
	}
	bool JCBuffer::create(int p_nLen)
    {
		m_nLen = p_nLen;
		if( m_pPtr && m_bNeedDel )
        {
			delete [] m_pPtr;
		}
		m_pPtr = new char [m_nLen];
		m_bNeedDel=true;
		return m_pPtr!=0;
	}
	void JCBuffer::free()
    {
		m_nLen = 0;
		if(m_bNeedDel && m_pPtr)
        {
			m_bNeedDel=false;
			delete [] m_pPtr;
			m_pPtr = 0;
		}
	}
	bool JCBuffer::toString(encode p_enc)
    {
		return false;
	}


	Buffer::Buffer(size_t size)
	{
		alloc(size);
	}

	Buffer::Buffer(const void* data, size_t size)
	{
		DEBUG_CHECK(data != nullptr || size != 0);
		alloc(size);
		if (m_data != nullptr) 
		{
			memcpy(m_data, data, size);
		}
	}

	Buffer::Buffer(std::shared_ptr<Data> data) : Buffer(data->data(), data->size())
	{
	}

	Buffer::~Buffer() 
	{
		delete[] m_data;
	}

	bool Buffer::alloc(size_t size) 
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
			m_size = 0;
		}
		m_data = size > 0 ? new (std::nothrow) uint8_t[size] : nullptr;
		if (m_data != nullptr) 
		{
			m_size = size;
		}
		return m_data != nullptr;
	}

	std::shared_ptr<Data> Buffer::release() 
	{
		if (isEmpty()) 
		{
			return nullptr;
		}
		auto data = Data::makeAdopted(m_data, m_size, Data::DeleteProc);
		m_data = nullptr;
		m_size = 0;
		return data;
	}

	void Buffer::reset() 
	{
		if (isEmpty())
		{
			return;
		}
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
	}

	void Buffer::clear()
	{
		if (isEmpty()) 
		{
			return;
		}
		memset(m_data, 0, m_size);
	}

	std::shared_ptr<Data> Buffer::copyRange(size_t offset, size_t length) 
	{
		length = getClampedLength(offset, length);
		if (length == 0) 
		{
			return nullptr;
		}
		return Data::makeWithCopy(m_data + offset, length);
	}

	void Buffer::writeRange(size_t offset, size_t length, const void* bytes)
	{
		length = getClampedLength(offset, length);
		if (length == 0) 
		{
			return;
		}
		memcpy(m_data + offset, bytes, length);
	}

	uint8_t Buffer::operator[](size_t index) const
	{
		DEBUG_CHECK(index >= 0 && index < m_size);
		return m_data[index];
	}

	uint8_t& Buffer::operator[](size_t index) 
	{
		DEBUG_CHECK(index >= 0 && index < m_size);
		return m_data[index];
	}

	size_t Buffer::getClampedLength(size_t offset, size_t length) const
	{
		size_t available = m_size;
		if (offset >= available || length == 0)
		{
			return 0;
		}
		available -= offset;
		if (length > available) 
		{
			length = available;
		}
		return length;
	}
}


