#include "GLAttributeMap.h"
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
#include <utils/Log.h>

namespace laya
{
	GLAttributeMap::GLAttributeMap()
	{

	}
	GLAttributeMap::~GLAttributeMap()
	{
		m_vAttributeMap.clear();
	}
	void GLAttributeMap::setValue(const char* sName, int nLocation)
	{
		m_vAttributeMap[sName] = nLocation;
	}
}