#include "BufferStateBase.h"
#include <cassert>
#include "../RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"
#include "../RenderEngine/RenderEngine/WebGLEngine/GLVertexState.h"
#include "VertexBuffer2D.h"
#include "IndexBuffer2D.h"

namespace laya
{
	BufferStateBase* BufferStateBase::m_curBindedBufferState = nullptr;
	BufferStateBase::BufferStateBase(WebGLEngine* engine)
	{
		m_nativeVertexArrayObject = engine->createVertexState();
	}
	BufferStateBase::~BufferStateBase()
	{
		if (m_nativeVertexArrayObject != nullptr)
		{
			destroy();
		}
	}
	void BufferStateBase::bind()
	{
		//if (BufferStateBase::m_curBindedBufferState != this)
		{
			m_nativeVertexArrayObject->bindVertexArray();
			BufferStateBase::m_curBindedBufferState = this;
		}
	}
	void BufferStateBase::unBind() 
	{
		if (BufferStateBase::m_curBindedBufferState == this)
		{
			m_nativeVertexArrayObject->unbindVertexArray();
			BufferStateBase::m_curBindedBufferState = nullptr;
		}
		else
		{
			assert("BufferState: must call bind() function first.");
		}
	}
	bool BufferStateBase::isBind()
	{
		return BufferStateBase::m_curBindedBufferState == this;
	}
	void BufferStateBase::destroy()
	{
		m_nativeVertexArrayObject->destroy();
		m_nativeVertexArrayObject = 0;
	}
	void BufferStateBase::applyState(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffer, IndexBuffer2D* indexBuffer)
	{
		//this._vertexBuffers = vertexBuffers;
		//this._bindedIndexBuffer = indexBuffer;
		if (indexBuffer)
		{
			indexBuffer->unbind();//清空绑定
		}
		bind();
		m_nativeVertexArrayObject->applyVertexBuffer(stride, attribInfo, vertexBuffer);
		m_nativeVertexArrayObject->applyIndexBuffer(indexBuffer);
		unBind();
		if (indexBuffer)
		{
			indexBuffer->unbind();//清空绑定
		}
	}
}
//------------------------------------------------------------------------------
