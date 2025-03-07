#ifndef __RTSubShader_H__
#define __RTSubShader_H__

#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <vector>
#include <unordered_map>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
namespace laya
{
struct UniformProperty;
class RTSubShader
{
  public:
    RTSubShader();
    ~RTSubShader();
    void addShaderPass(RTShaderPass *pass);
    void addUnifromProperty(int id, std::string propertyName, ShaderDataType type, int arrayLength);
       
    void destroy();

  public:
    std::vector<RTShaderPass *> shaderpasses;
    std::vector<UniformProperty> _uniformMap;
    bool enableInstance;
    std::string shaderName;
};
} // namespace laya

#endif //__SubShader_H__