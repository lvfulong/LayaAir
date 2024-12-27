#ifndef __GLBuffer_H__
#define __GLBuffer_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include <render/3D/design/renderEnum/BufferTargetType.h>

namespace laya
{
	class GLESEngine;



	class GLBuffer : public GLObject
	{
	public:
		GLBuffer(GLESEngine* engine, BufferTargetType targetType, BufferUsage bufferUsageType);
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
	public:
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