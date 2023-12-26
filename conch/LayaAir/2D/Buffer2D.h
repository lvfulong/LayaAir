#ifndef __Buffer2D_H__
#define __Buffer2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include "Buffer.h"

namespace laya
{
	class Buffer;
	class Buffer2D
	{	
	public:
		Buffer2D(Buffer* buffer);
		~Buffer2D();
		int getBufferLength();
		void setByteLength(int value);
		int needSize(int sz);
		Buffer2D& _resizeBuffer(int nsz, bool copy);//是否修改了长度
		void setNeedUpload() { m_upload = true; }
		bool _bind_upload();
		uint8_t* getData() { return &m_constBuffer->m_buffer[0]; }

	protected:
		void _bufferData();
		bool							m_upload = true;
		int								m_uploadSize = 0;		
		int								m_bufferSize = 0;
		Buffer*							m_constBuffer = nullptr;
	};
}
#endif //__Buffer2D_H__