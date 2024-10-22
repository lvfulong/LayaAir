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
    struct CacheShaderItem
    {
        GLESShaderInstance *_glesShaderInstance;
        jsbind::Persistent _jsShaderInstance;
    };
    RTShaderPass();
    ~RTShaderPass();
    void setCacheShader(RTDefineDatas *compileDefine, GLESShaderInstance *shader, jsbind::Persistent jsShaderInstance);
    void setCacheShaderJS(RTDefineDatas* compileDefine, GLESShaderInstance* shader, JSValueAsParam jsShaderInstanceWrapper);
    RTShaderPass::CacheShaderItem *getCacheShader(RTDefineDatas *compileDefine);
    JsValue getCacheShaderJS(RTDefineDatas *compileDefine);
    GLESShaderInstance *callCreateShaderInstanceFunction();
    void setRenderState(RenderState *value)
    {
        renderState = value;
    }
    void setValidDefine(RTDefineDatas *value)
    {
        validDefine = value;
    }
    void destroy();
    //void setCompileDefine(RTDefineDatas *value)
    //{
    //    _compileDefines = value;
    //}

  public:
    // binds
    void setCreateShaderInstanceFunction(JSValueAsParam value);
    // void createShaderInstance(RTDefineDatas *compileDefine);

  private:
    void _resizeCacheShaderMap(void *cacheMap, uint32_t hierarchy, uint32_t resizeLength);

  public:
    bool is2D = false;
    std::string pipelineMode;
    bool statefirst = false;
    RTDefineDatas *validDefine = nullptr;
    RenderState *renderState = nullptr;
    std::vector<std::string> nodeCommonMap;
    //RTDefineDatas *_compileDefines = nullptr;

  private:
    std::unordered_map<uint32_t, void *> _cacheSharders{};
    uint32_t _cacheShaderHierarchy = 1;
    jsbind::Persistent m_createShaderInstanceFunctionJS;
};
} // namespace laya

#endif