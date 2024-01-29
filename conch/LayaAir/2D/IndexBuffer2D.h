#ifndef __IndexBuffer2D_H__
#define __IndexBuffer2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include "Buffer2D.h"
#include "Buffer.h"

namespace laya
{
	class GLESEngine;
	class IndexBuffer2D : public Buffer
	{	
	public:
		IndexBuffer2D(GLESEngine* engine, BufferUsage bufferUsage);
		~IndexBuffer2D();
		void _bindForVAO();
	public:
		Buffer2D*						m_buffer2D;
	};
}
#endif //__IndexBuffer2D_H__
