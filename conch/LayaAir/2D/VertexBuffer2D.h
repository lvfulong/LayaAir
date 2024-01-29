#ifndef __VertexBuffer2D_H__
#define __VertexBuffer2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include "Buffer2D.h"
#include "Buffer.h"


namespace laya
{
	class GLESEngine;
	class VertexBuffer2D: public Buffer
	{	
	public:
		VertexBuffer2D(GLESEngine* engine, int vertexStride, BufferUsage bufferUsage);
		~VertexBuffer2D();
		void _bindForVAO();
	public:
		 int							m_vertexStride = 0;
		 Buffer2D*						m_buffer2D;
	};
}
#endif //__VertexBuffer2D_H__