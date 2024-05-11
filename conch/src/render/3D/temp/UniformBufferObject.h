#ifndef __UniformBufferObject_H__
#define __UniformBufferObject_H__

#include <stdio.h>
#include <unordered_map>
#include <render/3D/temp/ObjectBase.h>

namespace laya
{
	class GLESEngine;
	class GLBuffer;
	class UniformBufferObject : public ObjectBase<UniformBufferObject>
	{
	public:

		UniformBufferObject(GLESEngine* pEngine, int glPointer, bool bSelfCreateBuffer);

		~UniformBufferObject();

		void setGLBuffer(GLBuffer* pBuffer);
		
		void _bindUniformBufferBase();

	public:
		GLBuffer*	m_pBuffer = nullptr;

		int				m_glPointer;

		bool			m_bSelfCreateBuffer;
	};
}
#endif //__UniformBufferObject_H__
