#include "GLStateMap.h"
#include <unordered_map>
#include <assert.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include <Utils/Log.h>

namespace laya
{
	GLStateMap::GLStateMap()
	{

	}
	GLStateMap::~GLStateMap()
	{
		m_vStateMap.clear();
	}
	void GLStateMap::setValue(int key, int value)
	{
		m_vStateMap[key] = value;
	}
	int GLStateMap::getData(int key)
	{
		std::unordered_map<int, int>::iterator it = m_vStateMap.find(key);
		if (it != m_vStateMap.end())
		{
			return it->second;
		}
		return -1;
	}
}