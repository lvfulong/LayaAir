#include "GLObject.h"

namespace laya
{
	GLObject::GLObject(WebGLEngine* engine)
	{
		m_engine = engine;
		//m_id = m_engine.m_IDCounter++;
	}
	GLObject::~GLObject() 
	{}
}