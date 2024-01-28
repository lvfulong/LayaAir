#ifndef __Buffer_H__
#define __Buffer_H__



#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <iostream>
#include <vector>

namespace laya
{
    class GLBuffer;
	class WebGLEngine;
	class Buffer
	{
	public:
		Buffer(WebGLEngine* engine, BufferTargetType targetType, BufferUsage bufferUsageType);
		virtual ~Buffer();
		bool bind();
		void unbind();
		void resizelength(int bytelength);
	public:
		GLBuffer*				m_glBuffer = nullptr;
		std::vector<uint8_t>		m_buffer;
		BufferTargetType			m_bufferType;
		BufferUsage					m_bufferUsage;
		int							m_byteLength = 0;//upload bytelength
	};
}
#endif
