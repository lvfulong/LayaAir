#ifndef __BufferStateBase_H__
#define __BufferStateBase_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLVertexState.h>

namespace laya
{
	class GLESEngine;
	class VertexBuffer2D;
	class IndexBuffer2D;
	class GLVertexState;
	class BufferStateBase
	{	
	public:
		BufferStateBase(GLESEngine* engine);
		~BufferStateBase();
		void bind();
		void unBind();
		bool isBind();
		void destroy();
		void applyState(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffer, IndexBuffer2D* indexBuffer);
	public:
		GLVertexState*				m_nativeVertexArrayObject = nullptr;
		static BufferStateBase*			m_curBindedBufferState;
	};
}
#endif //__BufferStateBase_H__