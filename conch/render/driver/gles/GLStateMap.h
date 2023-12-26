#ifndef __GLStateMap_H__
#define __GLStateMap_H__

#include <string>
#include <unordered_map>

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
	class GLStateMap
	{
	public:
		GLStateMap();

		~GLStateMap();

		void setValue(int key, int value);
		std::unordered_map<int, int>& getStateMap() { return m_vStateMap; }
		int getData(int key);

		bool m_shaderPassStatefirst = false;
	private:
		std::unordered_map<int, int> m_vStateMap;

	};
}
#endif //__GLStateMap_H__