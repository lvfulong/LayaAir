#ifndef __VertexBuffer3D_H__
#define __VertexBuffer3D_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include <render/driver/gles/GLBuffer.h>
#include "VertexBuffer.h"

namespace laya
{
	class VertexBuffer3D: public VertexBuffer
	{
	public:
		VertexBuffer3D(WebGLEngine* engine,int byteLength, BufferUsage bufferUsageType,bool bSelfCreateBuffer);
		~VertexBuffer3D();
		void bind() override;
		void unbind() override;
		void setGLBuffer(IRenderBuffer* pBuffer);
		void orphanStorage();
		void setData(const char* buffer, int bufferOffset/* = 0*/, int dataStartIndex/* = 0*/, int dataCount/*= Number.MAX_SAFE_INTEGER*/);
	public:
		bool			m_bSelfCreateBuffer;
		IRenderBuffer*	m_buffer;
		int				m_nByteLength;
	};
}
#endif //__VertexBuffer3D_H__
