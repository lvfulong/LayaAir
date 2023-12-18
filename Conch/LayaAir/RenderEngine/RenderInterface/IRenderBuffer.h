#ifndef __IRenderBuffer_H__
#define __IRenderBuffer_H__

#include "../RenderEngine/WebGLEngine/GLObject.h"

namespace laya
{
	class IRenderBuffer
	{
	public:
		virtual ~IRenderBuffer() {}
		virtual bool bindBuffer() = 0;
		virtual void unbindBuffer() = 0;
		virtual void setDataLength(int length) = 0;
		virtual void setData(const char* buffer, int length, int offset) = 0;
		virtual void setDataEx(const char* buffer, int offset, int length) = 0;
		virtual void bindBufferBase(int glPointer) = 0;
		virtual void bindBufferRange(int glPointer, int offset, int byteCount) = 0;
		virtual void resizeBuffer(int dataLength) = 0;
		virtual void destroy() = 0;
	};
}
#endif