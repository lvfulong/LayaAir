#ifndef __VertexBuffer_H__
#define __VertexBuffer_H__

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include "../RenderEngine/RenderEngine/WebGLEngine/GLBuffer.h"

namespace laya
{
	struct VertexDeclaration
	{
		int location;
		int size;
		int type;
		int normalize;
		int stride;
		int offset;
	};
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer();
		void setVertexDeclaration( int* declaration,int intLength);
		virtual void bind() = 0;
		virtual void unbind() = 0;
		VertexDeclaration*			m_pVertextDeclaration = nullptr;
		int							m_nVertextDeclarationNum = 0;;
		bool						m_bInstanceBuffer = false;
	};
}
#endif //__VertexBuffer_H__
