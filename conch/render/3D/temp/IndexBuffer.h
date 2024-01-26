#ifndef _IndexBuffer_H__
#define _IndexBuffer_H__

#include <stdio.h>
#include <utils/Preprocessor.h>

namespace laya
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}
#endif //_IndexBuffer_H__
