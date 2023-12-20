#ifndef __VertexBuffer3D_H__
#define __VertexBuffer3D_H__

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
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
