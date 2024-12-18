#include "RTSubShader.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h"
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
void RTSubShader::addUnifromProperty(int id, std::string propertyName, ShaderDataType type, int arrayLength)
{
    _uniformMap.push_back(UniformProperty(id, propertyName, type, arrayLength));
}
void RTSubShader::destroy()
{
    // todo
}
} // namespace laya
