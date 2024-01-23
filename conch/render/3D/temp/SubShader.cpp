#include "SubShader.h"

namespace laya
{
	SubShader::SubShader()
	{
	}
	SubShader::~SubShader()
	{
	}
	void SubShader::addShaderPass(ShaderPass* pass)
	{
		shaderpasses.push_back(pass);
	}
	void SubShader::destroy()
	{
		//todo
	}
} // namespace laya
