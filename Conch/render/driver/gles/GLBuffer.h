#ifndef __GlBuffer_H__
#define __GlBuffer_H__

#include "GLObject.h"
#include "../../RenderEnum/BufferTargetType.h"
#include "../../RenderInterface/IRenderBuffer.h"

namespace laya
{
	class WebGLEngine;
	class GLBuffer : public GLObject, public IRenderBuffer
	{
	public:
		GLBuffer(WebGLEngine* engine, BufferTargetType targetType, BufferUsage bufferUsageType);
		~GLBuffer();
		bool bindBuffer();
		void unbindBuffer();
		void orphanStorage();
		void setDataLength(int length);
		void setData(const char* buffer, int length, int offset);
		void setDataEx(const char* buffer, int offset, int length);
		void bindBufferBase(int glPointer);
		void bindBufferRange(int glPointer, int offset, int byteCount);
		void resizeBuffer(int dataLength);
		void destroy();
	private:
		void _memorychange(int bytelength);
		void _getGLUsage(BufferUsage usage);
		void _getGLTarget(BufferTargetType target);
	private:
		//GLParams
		GLuint				m_glBuffer = 0;
		int					m_glTarget = 0;
		int					m_glUsage = 0;
		//Common Enum
		BufferTargetType	m_targetType;
		BufferUsage			m_bufferUsageType;
		//size
		int					m_byteLength = 0;
	};
}
#endif