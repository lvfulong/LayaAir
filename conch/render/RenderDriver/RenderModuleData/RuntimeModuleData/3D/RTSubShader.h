#ifndef __RTSubShader_H__
#define __RTSubShader_H__

#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <vector>

namespace laya
{

class RTSubShader
{
  public:
    RTSubShader();
    ~RTSubShader();
    void addShaderPass(RTShaderPass *pass);
    void destroy();

  public:
    std::vector<RTShaderPass *> shaderpasses;
};
} // namespace laya

#endif //__SubShader_H__