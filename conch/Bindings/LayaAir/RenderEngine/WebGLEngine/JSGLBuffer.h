#ifndef __JSGLBuffer_H__
#define __JSGLBuffer_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLBuffer.h>

namespace laya
{
	class JSGLBuffer
	{
	public:
		static void exportJS(Context& context);
		JSGLBuffer();
		JSGLBuffer(JSValueAsParam engine, int bufferTargetType, int bufferUsageType);
		~JSGLBuffer();

	public:

		bool bindBuffer();
		void unbindBuffer();
		void orphanStorage();
		void setDataLength(int length);
		void setData(JSValueAsParam buffer, int bufferOffset);
		void setDataEx(JSValueAsParam buffer,int offset, int length);
		void bindBufferBase(int glPointer);
		void bindBufferRange(int glPointer, int offset, int byteCount);
		void resizeBuffer(int length);
		void destroy();

	public:

		GLBuffer*	m_pGLBuffer;

	};
}
#endif //__JSGLBuffer_H__