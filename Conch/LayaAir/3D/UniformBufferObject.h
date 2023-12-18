#ifndef __UniformBufferObject_H__
#define __UniformBufferObject_H__

#include <stdio.h>
#include <unordered_map>
#include "../RenderEngine/ObjectBase.h"

namespace laya
{
	class WebGLEngine;
	class IRenderBuffer;
	class UniformBufferObject : public ObjectBase<UniformBufferObject>
	{
	public:

		UniformBufferObject(WebGLEngine* pEngine, int glPointer, bool bSelfCreateBuffer);

		~UniformBufferObject();

		void setGLBuffer(IRenderBuffer* pBuffer);
		
		void _bindUniformBufferBase();

	public:
		IRenderBuffer*	m_pBuffer = nullptr;

		int				m_glPointer;

		bool			m_bSelfCreateBuffer;
	};
}
#endif //__UniformBufferObject_H__
