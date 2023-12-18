#ifndef _IndexBuffer_H__
#define _IndexBuffer_H__

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif

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
