#include "SubShader.h"

namespace laya
{
	SubShader::SubShader()
	{
	}
	SubShader::~SubShader()
	{
	}
	void SubShader::addShaderpass(ShaderPass* pass)
	{
		shaderpasses.push_back(pass);
	}
} // namespace laya
