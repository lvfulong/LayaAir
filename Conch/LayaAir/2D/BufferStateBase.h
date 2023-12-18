#ifndef __BufferStateBase_H__
#define __BufferStateBase_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include "../RenderEngine/RenderInterface/IRenderVertexState.h"

namespace laya
{
	class IRenderVertexState;
	class WebGLEngine;
	class VertexBuffer2D;
	class IndexBuffer2D;
	class BufferStateBase
	{	
	public:
		BufferStateBase(WebGLEngine* engine);
		~BufferStateBase();
		void bind();
		void unBind();
		bool isBind();
		void destroy();
		void applyState(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffer, IndexBuffer2D* indexBuffer);
	public:
		IRenderVertexState*				m_nativeVertexArrayObject = nullptr;
		static BufferStateBase*			m_curBindedBufferState;
	};
}
#endif //__BufferStateBase_H__