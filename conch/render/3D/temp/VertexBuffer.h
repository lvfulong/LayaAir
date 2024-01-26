#ifndef __VertexBuffer_H__
#define __VertexBuffer_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include <render/driver/gles/GLBuffer.h>

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
