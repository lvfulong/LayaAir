#ifndef __RTShaderPass_H__
#define __RTShaderPass_H__
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h"
#include <binder/JSInterface.h>
#include <string>
#include <unordered_map>

namespace laya
{

class RenderState;
class RTShaderPass
{
  public:
    RTShaderPass();
    ~RTShaderPass();
    void setCacheShader(DefineDatas *compileDefine, ShaderInstance *shader);
    ShaderInstance *getCacheShader(DefineDatas *compileDefine);
    void callCreateShaderInstanceFunction();
    void setRenderState(RenderState *value)
    {
        renderState = value;
    }
    void destroy();

  public:
    // binds
    void setCreateShaderInstanceFunction(JSValueAsParam value);
    void createShaderInstance(DefineDatas *compileDefine);

  private:
    void _resizeCacheShaderMap(void *cacheMap, uint32_t hierarchy, uint32_t resizeLength);

  public:
    std::string pipelineMode;
    bool statefirst = false;
    DefineDatas validDefine;
    RenderState *renderState = nullptr;
    std::vector<std::string> nodeCommonMap;

  private:
    std::unordered_map<uint32_t, void *> _cacheSharders{};
    uint32_t _cacheShaderHierarchy = 1;
    Persistent m_createShaderInstanceFunctionJS;
};
} // namespace laya

#endif