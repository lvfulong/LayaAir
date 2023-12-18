#ifndef __ShaderVariable_H__
#define __ShaderVariable_H__

#include <string>


#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include <functional>
#include "../../3D/ShaderData.h"
namespace laya
{
	class ShaderVariable
	{
	public:
		std::string name;
		GLenum type;
		int location;
		bool isArray;
		int textureID;
		int dataOffset;
		int count;
		//caller:any;
		int byteSize = 0;
		std::function<int(ShaderVariable*, ShaderData::DataInfo*)> fun;
		std::vector<uint8_t> uploadedValue;
		ShaderVariable()
		{
			textureID = -1;
		}
		~ShaderVariable()
		{
			uploadedValue.clear();
		}
	};
}
#endif


