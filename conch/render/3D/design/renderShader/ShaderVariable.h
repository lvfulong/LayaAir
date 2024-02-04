#ifndef __ShaderVariable_H__
#define __ShaderVariable_H__

#include <string>
#include <utils/Preprocessor.h>
#include <functional>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
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
		std::function<int(ShaderVariable*, std::any*)> fun;
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


