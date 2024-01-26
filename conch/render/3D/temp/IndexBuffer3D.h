#ifndef _IndexBuffer3D_H__
#define _IndexBuffer3D_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include <render/driver/gles/GLBuffer.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include "IndexBuffer.h"

namespace laya
{
	class IndexBuffer3D : public IndexBuffer
	{
	public:
		IndexBuffer3D(WebGLEngine* engine, IndexFormat indexFormat,int indexCount,BufferUsage bufferUsageType,bool bSelfCreateBuffer);
		~IndexBuffer3D();
		void bind() override;
		void unbind() override;
		void setGLBuffer( IRenderBuffer* pBuffer);
	public:
		bool			m_bSelfCreateBuffer;
		IndexFormat		m_nIndexFormat;
		int				m_nIndexCount;
		int				m_nIndexTypeByteCount;
		int				m_nByteLength;
		IRenderBuffer*	m_buffer;
	};
}
#endif //_IndexBuffer3D_H__
