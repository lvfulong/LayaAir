#include "UniformBufferObject.h"
#include <cassert>
#include <Utils/Log.h>
#include "../../JCConch.h"
#include "../../JCConchRender.h"
#include "../RenderEngine/RenderInterface/IRenderBuffer.h"

namespace laya
{
	UniformBufferObject::UniformBufferObject(WebGLEngine* pEngine, int glPointer, bool bSelfCreateBuffer)
		:ObjectBase(JCConch::s_pConchRender->m_pUniformBufferObjectManager),
		m_glPointer(glPointer),
		m_bSelfCreateBuffer(bSelfCreateBuffer)
	{
		
	}
	UniformBufferObject::~UniformBufferObject()
	{
		if (m_bSelfCreateBuffer)
		{
			if (m_pBuffer != nullptr)
			{
				delete m_pBuffer;
				m_pBuffer = nullptr;
			}
		}
	}
	void UniformBufferObject::setGLBuffer(IRenderBuffer* pBuffer)
	{
		m_pBuffer = pBuffer;
	}
	void UniformBufferObject::_bindUniformBufferBase()
	{
		m_pBuffer->bindBufferBase(m_glPointer);
	}
}
//------------------------------------------------------------------------------
