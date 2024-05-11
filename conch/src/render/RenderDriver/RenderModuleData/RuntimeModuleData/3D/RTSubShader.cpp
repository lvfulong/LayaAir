#include "RTSubShader.h"

namespace laya
{
RTSubShader::RTSubShader()
{
}
RTSubShader::~RTSubShader()
{
}
void RTSubShader::addShaderPass(RTShaderPass *pass)
{
    shaderpasses.push_back(pass);
}
void RTSubShader::destroy()
{
    // todo
}
} // namespace laya
