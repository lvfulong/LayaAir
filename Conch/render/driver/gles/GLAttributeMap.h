#ifndef __GLAttributeMap_H__
#define __GLAttributeMap_H__

#include <string>
#include <unordered_map>
#include <map>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif

namespace laya
{
	class GLAttributeMap
	{
	public:
		GLAttributeMap();

		~GLAttributeMap();

		void setValue(const char* sName, int location);
		std::map<std::string, int>& getAttributeMap() { return m_vAttributeMap; }
	private:
		std::map<std::string, int> m_vAttributeMap;
	};
}
#endif //__GLAttributeMap_H__